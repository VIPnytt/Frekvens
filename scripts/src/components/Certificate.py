import cryptography.hazmat.primitives.hashes
import cryptography.hazmat.primitives.serialization
import cryptography.x509
import cryptography.x509.oid
import logging
import os
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
    certificates: list[cryptography.x509.Certificate]
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        self.certificates = []
        self.project = project

    def configure(self) -> None:
        self.certificates = []
        for option, hosts in {
            GoogleWeather.ENV_OPTION: GoogleWeather.HOST_WHITELIST,
            OpenMeteo.ENV_OPTION: OpenMeteo.HOST_WHITELIST,
            OpenWeather.ENV_OPTION: OpenWeather.HOST_WHITELIST,
            WorldWeatherOnline.ENV_OPTION: WorldWeatherOnline.HOST_WHITELIST,
            WttrIn.ENV_OPTION: WttrIn.HOST_WHITELIST,
            Yr.ENV_OPTION: Yr.HOST_WHITELIST,
        }.items():
            if option in self.project.dotenv and self.project.dotenv[option] == "true":
                for host in hosts:
                    for attempt in range(1, 4):
                        try:
                            self._add_host(host)
                            break
                        except (ConnectionError, ssl.SSLEOFError, TimeoutError) as e:
                            if attempt >= 3:
                                raise
                            logging.warning("%s (attempt #%d): %s", host, attempt, e)
        bundle_path = pathlib.Path("firmware/certs/bundle")
        bundle_path.mkdir(parents=True, exist_ok=True)
        ca_roots = "ca_roots.pem"
        with open(bundle_path / ca_roots, "w", encoding="utf-8") as pem:
            pem.write(self._get_pem())

    def finalize(self) -> None:
        self.certificates = []
        bundle_path = pathlib.Path("firmware/certs/bundle")
        for bundle_file in os.listdir(bundle_path):
            if bundle_file.endswith(".der"):
                self.add_der(bundle_path / bundle_file)
            elif bundle_file.endswith(".pem"):
                self._add_pem(bundle_path / bundle_file)
        embed = pathlib.Path("firmware/embed")
        embed.mkdir(parents=True, exist_ok=True)
        x509_crt_bundle = "x509_crt_bundle.bin"
        with open(embed / x509_crt_bundle, "wb") as bin:
            raw = self._get_bin()
            bin.write(raw if raw else b"\x00")

    def add_der(self, path: pathlib.Path | str) -> None:
        with open(path, "rb") as der:
            self.certificates.append(
                cryptography.x509.load_der_x509_certificate(der.read())
            )

    def _add_host(self, hostname: str, port: int = 443) -> bool:
        with socket.create_connection((hostname, port)) as sock:
            ctx = ssl.create_default_context()
            ctx.minimum_version = ssl.TLSVersion.TLSv1_2
            with ctx.wrap_socket(sock, server_hostname=hostname) as ssock:
                der = ssock.getpeercert(True)
                if der:
                    for pem in self._fetch_chain(der):
                        certificate = cryptography.x509.load_pem_x509_certificate(
                            pem.encode()
                        )
                        if self._is_self_signed(pem):
                            if not any(
                                _certificate.fingerprint(
                                    cryptography.hazmat.primitives.hashes.SHA256()
                                ).hex()
                                == certificate.fingerprint(
                                    cryptography.hazmat.primitives.hashes.SHA256()
                                ).hex()
                                for _certificate in self.certificates
                            ):
                                self.certificates.append(certificate)
                            return True
        return False

    def _add_pem(self, path: pathlib.Path | str) -> None:
        with open(path, encoding="utf-8") as pem:
            certificate = ""
            encoded = False
            for line in [line.rstrip("\r\n") for line in pem.readlines()]:
                if line == "-----BEGIN CERTIFICATE-----" and encoded is False:
                    certificate = ""
                    encoded = True
                elif line == "-----END CERTIFICATE-----" and encoded is True:
                    certificate += line
                    encoded = False
                    self.certificates.append(
                        cryptography.x509.load_pem_x509_certificate(
                            certificate.encode()
                        )
                    )
                if encoded is True:
                    certificate += line

    def _get_bin(self) -> bytes:
        self.certificates = sorted(
            self.certificates, key=lambda cert: cert.subject.public_bytes()
        )
        offsets = []
        bundle = b""
        for certificate in self.certificates:
            public_key_der = certificate.public_key().public_bytes(
                cryptography.hazmat.primitives.serialization.Encoding.DER,
                cryptography.hazmat.primitives.serialization.PublicFormat.SubjectPublicKeyInfo,
            )
            subject_name_der = certificate.subject.public_bytes()
            offsets.append(4 * len(self.certificates) + len(bundle))
            bundle += (
                struct.pack("<HH", len(subject_name_der), len(public_key_der))
                + subject_name_der
                + public_key_der
            )
        return struct.pack("<{0:d}L".format(len(offsets)), *offsets) + bundle

    def _get_pem(self) -> str:
        bundle = ""
        for cert in self.certificates:
            pem = cert.public_bytes(
                cryptography.hazmat.primitives.serialization.Encoding.PEM
            ).decode()
            name = self._get_name(pem)
            if len(bundle):
                bundle += "\n"
            if name:
                bundle += f"{name}\n"
                bundle += f"{'=' * len(name)}\n"
            bundle += pem.strip() + "\n"
        return bundle

    def _fetch_chain(self, der: bytes) -> list[str]:
        pem_list = []
        fingerprints = set()
        while der:
            cert = cryptography.x509.load_der_x509_certificate(der)
            fingerprint = cert.fingerprint(
                cryptography.hazmat.primitives.hashes.SHA256()
            ).hex()
            if fingerprint in fingerprints:
                break
            fingerprints.add(fingerprint)
            pem_list.append(ssl.DER_cert_to_PEM_cert(der))
            if cert.issuer == cert.subject:
                break
            _der = self._fetch_issuer(cert)
            if _der is None:
                break
            der = _der
        return pem_list

    def _fetch_issuer(self, cert: cryptography.x509.Certificate) -> bytes | None:
        try:
            for value in typing.cast(
                cryptography.x509.AuthorityInformationAccess,
                cert.extensions.get_extension_for_oid(
                    cryptography.x509.oid.ExtensionOID.AUTHORITY_INFORMATION_ACCESS
                ).value,
            ):
                if (
                    value.access_method.dotted_string == "1.3.6.1.5.5.7.48.2"
                    and value.access_location.value.startswith("http://")
                ):
                    return urllib.request.urlopen(value.access_location.value).read()
        except Exception as e:
            logging.warning("Failed to fetch issuer certificate: %s", e)
        return None

    def _is_self_signed(self, pem: str) -> bool:
        try:
            cert = cryptography.x509.load_pem_x509_certificate(pem.encode())
            return cert.issuer == cert.subject
        except Exception as e:
            logging.warning("Failed to load issuer certificate: %s", e)
        return False

    def _get_name(self, pem: str) -> str | None:
        subject = cryptography.x509.load_pem_x509_certificate(pem.encode()).subject
        com = subject.get_attributes_for_oid(cryptography.x509.oid.NameOID.COMMON_NAME)
        if com and isinstance(com[0].value, str):
            return com[0].value
        org = subject.get_attributes_for_oid(
            cryptography.x509.oid.NameOID.ORGANIZATION_NAME
        )
        if org and isinstance(org[0].value, str):
            return org[0].value
        return None
