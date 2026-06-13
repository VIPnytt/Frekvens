import dotenv
import os
import pathlib
import re
import shutil
import typing
import unicodedata

from .components.Dependency import Dependency
from .components.Deprecated import Deprecated
from .components.Partition import Partition
from .components.Time import Time
from .config.version import VERSION
from .devices.IkeaFrekvens import IkeaFrekvens
from .devices.IkeaObegransad import IkeaObegransad
from .extensions.Ota import Ota
from .extensions.WebApp import WebApp
from .modes.Weather import Weather
from .Extra import Extra
from .Firmware import Firmware
from .Tools import Tools

if typing.TYPE_CHECKING:
    from .components.Types import Environment, COMMAND_LINE_TARGETS  # noqa: E402
else:
    from SCons.Script import Environment, COMMAND_LINE_TARGETS  # noqa: E402


class Frekvens:
    dependency: Dependency | None = None
    deprecated: Deprecated
    dotenv: dict[str, str]
    env: Environment
    extra: Extra
    firmware: Firmware | None = None
    ota: Ota | None = None
    partition: Partition
    time: Time | None = None
    tools: Tools
    weather: Weather | None = None
    webapp: WebApp | None = None

    def __init__(self, env: Environment) -> None:
        self.env = env
        self.deprecated = Deprecated(self)
        self.extra = Extra(self)
        self.ota = Ota(self)
        self.partition = Partition(self)
        self.tools = Tools(self)
        if COMMAND_LINE_TARGETS not in [
            ["buildfs"],
            ["compiledb"],
            ["uploadfs"],
            ["uploadfsota"],
        ]:
            self.dependency = Dependency(self)
        if COMMAND_LINE_TARGETS not in [
            ["buildfs"],
            ["uploadfs"],
            ["uploadfsota"],
        ]:
            self.firmware = Firmware(self)
            self.weather = Weather(self)
        if COMMAND_LINE_TARGETS not in [
            ["buildfs"],
            ["uploadfs"],
            ["uploadfsota"],
        ]:
            self.time = Time(self)
        if COMMAND_LINE_TARGETS not in [
            ["build"],
            ["compiledb"],
            ["upload"],
        ]:
            self.webapp = WebApp(self)
        self.dotenv = {key: (value if value is not None else "") for key, value in dotenv.dotenv_values(".env").items()}
        if "NAME" not in self.dotenv:
            if IkeaFrekvens.ENV_OPTION in self.dotenv and self.dotenv[IkeaFrekvens.ENV_OPTION] == "true":
                self.dotenv["NAME"] = IkeaFrekvens.NAME
            elif IkeaObegransad.ENV_OPTION in self.dotenv and self.dotenv[IkeaObegransad.ENV_OPTION] == "true":
                self.dotenv["NAME"] = IkeaObegransad.NAME
            else:
                self.dotenv["NAME"] = "Frekvens"
        if "HOSTNAME" in self.dotenv:
            self.dotenv["HOSTNAME"] = self.dotenv["HOSTNAME"].lower()
        else:
            self.dotenv["HOSTNAME"] = re.sub(
                r"[^a-z0-9]+",
                "-",
                unicodedata.normalize("NFKD", self.dotenv["NAME"]).encode("ascii", "ignore").decode("ascii").lower(),
            ).strip("-")

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
        self.deprecated.initialize()
        self.partition.initialize()

    def configure(self) -> None:
        if self.ota:
            self.ota.configure()
        if self.time:
            self.time.configure()
        if self.weather:
            self.weather.configure()

    def validate(self) -> None:
        if self.ota:
            self.ota.validate()
        if self.weather:
            self.weather.validate()
        if self.webapp:
            self.webapp.validate()

    def finalize(self) -> None:
        if self.firmware:
            self.firmware.finalize()
        if self.ota:
            self.ota.finalize()
        if self.webapp:
            self.webapp.finalize()

    @staticmethod
    def clean() -> None:
        Deprecated.clean()
        Extra.clean()
        WebApp.clean()
        Tools.clean()
        for path in {
            "logs",
            "scripts/__pycache__",
        }:
            if os.path.exists(path):
                shutil.rmtree(path, ignore_errors=True)
                print(f"Removing {path}")
