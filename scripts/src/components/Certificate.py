import cryptography.hazmat.primitives.hashes
import cryptography.hazmat.primitives.serialization
import cryptography.x509
import cryptography.x509.oid
import logging
import pathlib
import socket
import struct
import ssl
import typing
import urllib.request

from ..modes.GoogleWeather import GoogleWeather
from ..modes.OpenMeteo import OpenMeteo
from ..modes.OpenWeather import OpenWeather
from ..modes.WorldWeatherOnline import WorldWeatherOnline
from ..modes.WttrIn import WttrIn
from ..modes.Yr import Yr

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens


class Certificate:
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
                    except (ConnectionError, ssl.SSLEOFError, TimeoutError) as e:
                        if attempt >= 3:
                            raise
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
                if not (der := ssock.getpeercert(True)):
                    return
                for pem in self._fetch_chain(der):
                    cert = cryptography.x509.load_pem_x509_certificate(pem.encode())
                    if cert.issuer == cert.subject:
                        self._append_certificate(cert)
                        return

    def _add_pem(self, path: pathlib.Path) -> None:
        lines: list[str] = []
        inside = False
        for line in path.read_text().splitlines():
            if line == "-----BEGIN CERTIFICATE-----":
                lines = [line]
                inside = True
            elif line == "-----END CERTIFICATE-----" and inside:
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

    def _fetch_chain(self, der: bytes) -> list[str]:
        pem_list: list[str] = []
        fingerprints: set[bytes] = set()
        current_der = der
        while current_der:
            cert = cryptography.x509.load_der_x509_certificate(current_der)
            fingerprint = cert.fingerprint(cryptography.hazmat.primitives.hashes.SHA256())
            if fingerprint in fingerprints:
                break
            fingerprints.add(fingerprint)
            pem_list.append(ssl.DER_cert_to_PEM_cert(current_der))
            if cert.issuer == cert.subject:
                break
            if (issuer_der := self._fetch_issuer(cert)) is None:
                break
            current_der = issuer_der
        return pem_list

    def _fetch_issuer(self, cert: cryptography.x509.Certificate) -> bytes | None:
        try:
            for value in typing.cast(
                cryptography.x509.AuthorityInformationAccess,
                cert.extensions.get_extension_for_oid(
                    cryptography.x509.oid.ExtensionOID.AUTHORITY_INFORMATION_ACCESS
                ).value,
            ):
                if value.access_method.dotted_string == "1.3.6.1.5.5.7.48.2" and value.access_location.value.startswith(
                    "http://"
                ):
                    with urllib.request.urlopen(value.access_location.value) as response:
                        return response.read()
        except Exception as e:
            logging.warning("Failed to fetch issuer certificate: %s", e)
        return None

    def _get_name(self, cert: cryptography.x509.Certificate) -> str | None:
        com = cert.subject.get_attributes_for_oid(cryptography.x509.oid.NameOID.COMMON_NAME)
        if com and isinstance(com[0].value, str):
            return com[0].value
        org = cert.subject.get_attributes_for_oid(cryptography.x509.oid.NameOID.ORGANIZATION_NAME)
        if org and isinstance(org[0].value, str):
            return org[0].value
        return None
