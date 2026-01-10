import dotenv
import os
import pathlib
import SCons.Script
import shutil

from .components.Certificate import Certificate
from .components.Dependency import Dependency
from .components.Partition import Partition
from .components.TimeZone import TimeZone
from .config.version import VERSION
from .extensions.Ota import Ota
from .extensions.WebApp import WebApp
from .Extra import Extra
from .Firmware import Firmware
from .Tools import Tools

class Frekvens:
    certificate: Certificate | None = None
    dependency: Dependency | None = None
    dotenv: dict[str, str]
    env: SCons.Script.Environment
    extra: Extra
    firmware: Firmware | None = None
    ota: Ota | None = None
    partition: Partition
    timezone: TimeZone | None = None
    tools: Tools
    webapp: WebApp | None = None

    def __init__(self, env: SCons.Script.Environment) -> None:
        self.env = env
        self.extra = Extra(self)
        self.ota = Ota(self)
        self.partition = Partition(self)
        self.tools = Tools(self)
        if SCons.Script.COMMAND_LINE_TARGETS not in [
            ["build"],
            ["upload"],
        ]:
            self.webapp = WebApp(self)
        if SCons.Script.COMMAND_LINE_TARGETS not in [
            ["buildfs"],
            ["uploadfs"],
            ["uploadfsota"],
        ]:
            self.certificate = Certificate(self)
            self.dependency = Dependency(self)
            self.firmware = Firmware(self)
            self.timezone = TimeZone(self)
        self.dotenv = {
            key: (value if value is not None else "")
            for key, value in dotenv.dotenv_values(".env").items()
        }

    def run(self) -> None:
        print(f"Frekvens {VERSION}")
        pathlib.Path("data").mkdir(parents=True, exist_ok=True)
        self.initialize()
        self.configure()
        self.validate()
        self.finalize()

    def initialize(self) -> None:
        if self.dependency:
            self.dependency.initialize()
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
        if self.dependency:
            self.dependency.validate()
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
