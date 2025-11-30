import dotenv
import httpx
import os
import packaging.version
import pathlib
import shutil

from .config.version import VERSION
from .components.Certificate import Certificate
from .components.Partition import Partition
from .components.TimeZone import TimeZone
from .extensions.Ota import Ota
from .extensions.WebApp import WebApp
from .Extra import Extra
from .Firmware import Firmware
from .Tools import Tools


class Frekvens:
    certificate: Certificate | None = None
    dotenv: dict[str, str]
    extra: Extra
    firmware: Firmware | None = None
    ota: Ota | None = None
    partition: Partition
    timezone: TimeZone | None = None
    tools: Tools
    webapp: WebApp | None = None
    working: str = ""

    def __init__(self, env, targets: list[str]) -> None:
        self.env = env
        self.extra = Extra(self)
        self.ota = Ota(self)
        self.partition = Partition(self)
        self.tools = Tools(self)
        if targets not in [
            ["build"],
            ["upload"],
        ]:
            self.webapp = WebApp(self)
        if targets not in [
            ["buildfs"],
            ["uploadfs"],
            ["uploadfsota"],
        ]:
            self.certificate = Certificate(self)
            self.firmware = Firmware(self)
            self.timezone = TimeZone(self)
        self.dotenv = {
            key: (value if value is not None else "")
            for key, value in dotenv.dotenv_values(".env").items()
        }
        self.working = f"env:{ self.env['PIOENV']}"

    def run(self) -> None:
        releases = httpx.get(
            "https://api.github.com/repos/VIPnytt/Frekvens/releases/latest",
            headers={
                "Accept": "application/vnd.github+json",
            },
        )
        if releases.status_code == 200:
            current = packaging.version.parse(VERSION)
            latest = packaging.version.parse(releases.json()["tag_name"].lstrip("v"))
            yellow = "\033[93m"
            reset = "\033[0m"
            if latest > current:
                print(
                    f"{yellow}[notice] A new release of Frekvens is available: {current.public} -> {latest.public}{reset}"
                )
        print(f"Building Frekvens {VERSION}")
        pathlib.Path("data").mkdir(parents=True, exist_ok=True)
        self.initialize()
        self.configure()
        self.validate()
        self.finalize()

    def initialize(self) -> None:
        if self.firmware:
            self.firmware.initialize()
        self.tools.initialize()
        self.partition.initialize()
        if self.webapp:
            self.webapp.initialize()

    def configure(self) -> None:
        if self.certificate:
            self.certificate.configure()
        if self.ota:
            self.ota.configure()
        if self.timezone:
            self.timezone.configure()

    def validate(self) -> None:
        if self.ota:
            self.ota.validate()
        if self.webapp:
            self.webapp.validate()

    def finalize(self) -> None:
        if self.certificate:
            self.certificate.finalize()
        if self.firmware:
            self.firmware.finalize()
        if self.ota:
            self.ota.finalize()
        if self.webapp:
            self.webapp.finalize()

    @staticmethod
    def clean() -> None:
        Extra.clean()
        Firmware.clean()
        WebApp.clean()
        Tools.clean()
        for path in [
            "logs",
            "scripts/__pycache__",
        ]:
            if os.path.exists(path):
                shutil.rmtree(path, ignore_errors=True)
                print(f"Removing {path}")
