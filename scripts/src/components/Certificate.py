import cryptography.exceptions
import cryptography.hazmat.primitives.asymmetric.dsa
import cryptography.hazmat.primitives.asymmetric.ec
import cryptography.hazmat.primitives.asymmetric.ed25519
import cryptography.hazmat.primitives.asymmetric.ed448
import cryptography.hazmat.primitives.asymmetric.padding
import cryptography.hazmat.primitives.asymmetric.rsa
import cryptography.hazmat.primitives.hashes
import cryptography.hazmat.primitives.serialization
import cryptography.hazmat.primitives.serialization.pkcs7
import cryptography.x509
import cryptography.x509.oid
import logging
import pathlib
import socket
import struct
import ssl
import sys
import typing
import urllib.request
import warnings

from ..modes.GoogleWeather import GoogleWeather
from ..modes.OpenMeteo import OpenMeteo
from ..modes.OpenWeather import OpenWeather
from ..modes.WorldWeatherOnline import WorldWeatherOnline
from ..modes.WttrIn import WttrIn
from ..modes.Yr import Yr

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens


class Certificate:
    PEM_CERT_BEGIN = "-----BEGIN CERTIFICATE-----"
    PEM_CERT_END = "-----END CERTIFICATE-----"
    PEM_PKCS7_BEGIN = "-----BEGIN PKCS7-----"
    PROVIDERS: dict[str, list[str]] = {
        GoogleWeather.ENV_OPTION: GoogleWeather.HOST_WHITELIST,
        OpenMeteo.ENV_OPTION: OpenMeteo.HOST_WHITELIST,
        OpenWeather.ENV_OPTION: OpenWeather.HOST_WHITELIST,
        WorldWeatherOnline.ENV_OPTION: WorldWeatherOnline.HOST_WHITELIST,
        WttrIn.ENV_OPTION: WttrIn.HOST_WHITELIST,
        Yr.ENV_OPTION: Yr.HOST_WHITELIST,
    }
    certificates: list[cryptography.x509.Certificate]
    fingerprints: set[bytes]
    path = pathlib.Path("firmware") / "certs" / "bundle"
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        self.certificates = []
        self.fingerprints: set[bytes] = set()
        self.project = project
        self.path.mkdir(parents=True, exist_ok=True)

    def configure(self) -> None:
        self.certificates = []
        self.fingerprints = set()
        for option, hosts in self.PROVIDERS.items():
            if self.project.dotenv.get(option) != "true":
                continue
            for host in hosts:
                for attempt in range(1, 4):
                    try:
                        self._add_host(host)
                        break
                    except (ConnectionError, ssl.SSLError, TimeoutError) as e:
                        if attempt >= 3:
                            raise
                        if isinstance(e, ssl.SSLCertVerificationError):
                            logging.warning("%s (attempt #%d): %s", host, attempt, e.verify_message)
                        else:
                            logging.warning("%s (attempt #%d): %s", host, attempt, e)
        with open(self.path / "ca_roots.pem", "w", encoding="utf-8") as f:
            f.write(self._get_pem())

    def finalize(self) -> None:
        self.certificates = []
        self.fingerprints = set()
        for bundle in self.path.iterdir():
            if bundle.suffix == ".der":
                self._append_certificate(cryptography.x509.load_der_x509_certificate(bundle.read_bytes()))
            elif bundle.suffix == ".pem":
                self._add_pem(bundle)
        embed = pathlib.Path("firmware") / "embed"
        embed.mkdir(parents=True, exist_ok=True)
        raw = self._get_bin()
        with open(embed / "x509_crt_bundle.bin", "wb") as f:
            f.write(raw if raw else b"\x00")

    def _add_host(self, hostname: str, port: int = 443) -> None:
        with socket.create_connection((hostname, port)) as sock:
            ctx = ssl.create_default_context()
            ctx.minimum_version = ssl.TLSVersion.TLSv1_2
            with ctx.wrap_socket(sock, server_hostname=hostname) as ssock:
                if sys.version_info >= (3, 13):
                    for der in ssock.get_verified_chain():
                        cert = cryptography.x509.load_der_x509_certificate(der)
                        if self._is_root(cert):
                            self._append_certificate(cert)
                            return
                    logging.warning(
                        "Verified chain for %s did not include a root certificate, falling back to AIA", hostname
                    )
                if not (der := ssock.getpeercert(True)):
                    return
                for cert in self._fetch_chain(hostname, der):
                    if self._is_root(cert):
                        self._append_certificate(cert)
                        return
                logging.warning("No root certificate found for %s", hostname)

    def _add_pem(self, path: pathlib.Path) -> None:
        lines: list[str] = []
        inside = False
        for line in path.read_text().splitlines():
            if line == self.PEM_CERT_BEGIN:
                lines = [line]
                inside = True
            elif line == self.PEM_CERT_END and inside:
                lines.append(line)
                certificate = cryptography.x509.load_pem_x509_certificate("\n".join(lines).encode())
                self._append_certificate(certificate)
                inside = False
            elif inside:
                lines.append(line)

    def _append_certificate(self, cert: cryptography.x509.Certificate) -> None:
        if (fingerprint := cert.fingerprint(cryptography.hazmat.primitives.hashes.SHA256())) not in self.fingerprints:
            self.certificates.append(cert)
            self.fingerprints.add(fingerprint)
            logging.info("Added root certificate: %s", self._get_name(cert) or "unknown")

    def _get_bin(self) -> bytes:
        offsets: list[int] = []
        bundle = b""
        certificates = sorted(self.certificates, key=lambda cert: cert.subject.public_bytes())
        for cert in certificates:
            public_key_der = cert.public_key().public_bytes(
                cryptography.hazmat.primitives.serialization.Encoding.DER,
                cryptography.hazmat.primitives.serialization.PublicFormat.SubjectPublicKeyInfo,
            )
            subject_name_der = cert.subject.public_bytes()
            offsets.append(4 * len(certificates) + len(bundle))
            bundle += struct.pack("<HH", len(subject_name_der), len(public_key_der)) + subject_name_der + public_key_der
        return struct.pack("<{0:d}L".format(len(offsets)), *offsets) + bundle

    def _get_pem(self) -> str:
        blocks: list[str] = []
        certificates = sorted(self.certificates, key=lambda cert: cert.subject.public_bytes())
        for cert in certificates:
            pem = cert.public_bytes(cryptography.hazmat.primitives.serialization.Encoding.PEM).decode().strip()
            if name := self._get_name(cert):
                blocks.append(f"{name}\n{'=' * len(name)}\n{pem}")
            else:
                blocks.append(pem)
        return "\n\n".join(blocks) + ("\n" if blocks else "")

    def _fetch_chain(self, hostname: str, der: bytes) -> list[cryptography.x509.Certificate]:
        reached_root = False
        chain: list[cryptography.x509.Certificate] = []
        fingerprints: set[bytes] = set()
        current_der = der
        while current_der:
            try:
                cert = cryptography.x509.load_der_x509_certificate(current_der)
            except ValueError as e:
                logging.warning("%s: Invalid issuer certificate format: %s", hostname, e)
                break
            fingerprint = cert.fingerprint(cryptography.hazmat.primitives.hashes.SHA256())
            if fingerprint in fingerprints:
                break
            fingerprints.add(fingerprint)
            chain.append(cert)
            if self._is_root(cert):
                reached_root = True
                break
            current_der = self._fetch_issuer(hostname, cert)
            if current_der is None:
                break
        if chain and not reached_root:
            logging.warning(
                "%s: Did not reach root certificate for chain ending at: %s",
                hostname,
                self._get_name(chain[-1]) or "unknown",
            )
        return chain

    def _fetch_issuer(self, hostname: str, cert: cryptography.x509.Certificate) -> bytes | None:
        aia = typing.cast(
            cryptography.x509.AuthorityInformationAccess,
            cert.extensions.get_extension_for_oid(
                cryptography.x509.oid.ExtensionOID.AUTHORITY_INFORMATION_ACCESS
            ).value,
        )
        for value in aia:
            if value.access_method != cryptography.x509.oid.AuthorityInformationAccessOID.CA_ISSUERS:
                continue
            location = value.access_location.value
            if not isinstance(location, str) or not location.startswith(("http://", "https://")):
                continue
            data = self._download(hostname, location)
            if data is None:
                continue
            candidates = self._parse_certificates(hostname, location, data)
            issuer = self._select_issuer(cert, candidates)
            if issuer:
                return issuer.public_bytes(cryptography.hazmat.primitives.serialization.Encoding.DER)
            logging.warning("%s: Issuer certificate not found in response: %s", hostname, location)
        return None

    def _download(self, hostname: str, location: str) -> bytes | None:
        try:
            with urllib.request.urlopen(location) as response:
                return response.read()
        except (OSError, ValueError) as e:
            logging.warning("%s: Failed to download issuer: %s (%s)", hostname, location, e)
            return None

    def _parse_certificates(self, hostname: str, location: str, data: bytes) -> list[cryptography.x509.Certificate]:
        candidates: list[cryptography.x509.Certificate] = []
        if self.PEM_CERT_BEGIN.encode() in data:
            candidates.append(cryptography.x509.load_pem_x509_certificate(data))
            return candidates
        if self.PEM_PKCS7_BEGIN.encode() in data:
            logging.info("%s: Parsing PKCS#7 (PEM) from %s", hostname, location)
            return cryptography.hazmat.primitives.serialization.pkcs7.load_pem_pkcs7_certificates(data)
        try:
            candidates.append(cryptography.x509.load_der_x509_certificate(data))
            return candidates
        except ValueError:
            pass
        try:
            with warnings.catch_warnings(record=True) as caught:
                warnings.simplefilter("always")
                certs = cryptography.hazmat.primitives.serialization.pkcs7.load_der_pkcs7_certificates(data)
            for warning in caught:
                logging.warning("%s via %s: %s", hostname, location, warning.message)
            return certs
        except ValueError:
            logging.warning("%s: Unsupported issuer format: %s", hostname, location)
            return []

    def _select_issuer(
        self, cert: cryptography.x509.Certificate, candidates: list[cryptography.x509.Certificate]
    ) -> cryptography.x509.Certificate | None:
        for candidate in candidates:
            if candidate.subject != cert.issuer:
                continue
            bc = None
            try:
                bc = candidate.extensions.get_extension_for_class(cryptography.x509.BasicConstraints).value
            except cryptography.x509.ExtensionNotFound:
                logging.debug("Issuer candidate missing BasicConstraints: %s", self._get_name(candidate) or "unknown")
            if bc is not None and not bc.ca:
                continue
            return candidate
        return None

    def _is_root(self, cert: cryptography.x509.Certificate) -> bool:
        if cert.issuer != cert.subject:
            return False
        try:
            basic_constraints = cert.extensions.get_extension_for_class(cryptography.x509.BasicConstraints).value
        except cryptography.x509.ExtensionNotFound:
            return False
        if not basic_constraints.ca:
            return False
        public_key = cert.public_key()
        try:
            if isinstance(public_key, cryptography.hazmat.primitives.asymmetric.dsa.DSAPublicKey):
                algorithm = cert.signature_hash_algorithm
                if algorithm is None:
                    return False
                public_key.verify(
                    cert.signature,
                    cert.tbs_certificate_bytes,
                    algorithm,
                )
                return True
            if isinstance(public_key, cryptography.hazmat.primitives.asymmetric.ec.EllipticCurvePublicKey):
                algorithm = cert.signature_hash_algorithm
                if algorithm is None:
                    return False
                public_key.verify(
                    cert.signature,
                    cert.tbs_certificate_bytes,
                    cryptography.hazmat.primitives.asymmetric.ec.ECDSA(algorithm),
                )
                return True
            if isinstance(public_key, cryptography.hazmat.primitives.asymmetric.ed25519.Ed25519PublicKey):
                public_key.verify(cert.signature, cert.tbs_certificate_bytes)
                return True
            if isinstance(public_key, cryptography.hazmat.primitives.asymmetric.ed448.Ed448PublicKey):
                public_key.verify(cert.signature, cert.tbs_certificate_bytes)
                return True
            if isinstance(public_key, cryptography.hazmat.primitives.asymmetric.rsa.RSAPublicKey):
                algorithm = cert.signature_hash_algorithm
                if algorithm is None:
                    return False
                public_key.verify(
                    cert.signature,
                    cert.tbs_certificate_bytes,
                    cryptography.hazmat.primitives.asymmetric.padding.PKCS1v15(),
                    algorithm,
                )
                return True
        except (
            cryptography.exceptions.InvalidSignature,
            cryptography.exceptions.UnsupportedAlgorithm,
            TypeError,
            ValueError,
        ):
            return False
        return False

    def _get_name(self, cert: cryptography.x509.Certificate) -> str | None:
        com = cert.subject.get_attributes_for_oid(cryptography.x509.oid.NameOID.COMMON_NAME)
        if com and isinstance(com[0].value, str):
            return com[0].value
        org = cert.subject.get_attributes_for_oid(cryptography.x509.oid.NameOID.ORGANIZATION_NAME)
        if org and isinstance(org[0].value, str):
            return org[0].value
        return None
