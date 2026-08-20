import os
import re
import shutil
import typing
import unicodedata

import dotenv

from .components.Dependency import Dependency
from .components.Deprecated import Deprecated
from .components.Partition import Partition
from .components.Time import Time
from .config.version import VERSION
from .devices.IkeaFrekvens import IkeaFrekvens
from .devices.IkeaObegransad import IkeaObegransad
from .extensions.Ota import Ota
from .extensions.WebApp import WebApp
from .Extra import Extra
from .Firmware import Firmware
from .modes.Weather import Weather
from .Tools import Tools

if typing.TYPE_CHECKING:
    from .components.Types import Environment
else:
    from SCons.Script import Environment


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
        """
        Initialize the project orchestrator with the provided build environment.

        Parameters:
            env (Environment): SCons build environment used to configure project components.
        """
        self.env = env
        self.dotenv = {key: (value if value is not None else "") for key, value in dotenv.dotenv_values(".env").items()}
        self.dependency = Dependency(self)
        self.deprecated = Deprecated(self)
        self.extra = Extra(self)
        self.firmware = Firmware(self)
        self.partition = Partition(self)
        self.time = Time(self)
        self.tools = Tools(self)
        self.deprecated.migrate()
        if Ota.ENV_OPTION in self.dotenv and self.dotenv[Ota.ENV_OPTION] == "true":
            self.ota = Ota(self)
        if Weather.ENV_OPTION in self.dotenv and self.dotenv[Weather.ENV_OPTION] == "true":
            self.weather = Weather(self)
        if WebApp.ENV_OPTION in self.dotenv and self.dotenv[WebApp.ENV_OPTION] == "true":
            self.webapp = WebApp(self)

    def run(self) -> None:
        """
        Execute the complete Frekvens build lifecycle.
        """
        print(f"Frekvens {VERSION}")
        self.initialize()
        self.configure()
        self.validate()
        self.finalize()

    def initialize(self) -> None:
        """
        Initialize project settings and enabled components.

        Determines default application and host names, normalizes configured hostnames, and initializes the available project components.
        """
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
        if self.dependency:
            self.dependency.initialize()
        if self.firmware:
            self.firmware.initialize()
        self.partition.initialize()
        if self.time:
            self.time.initialize()
        if self.weather:
            self.weather.initialize()
        if self.webapp:
            self.webapp.initialize()

    def configure(self) -> None:
        """Configure the enabled OTA, time, and weather components."""
        if self.ota:
            self.ota.configure()
        if self.time:
            self.time.configure()
        if self.weather:
            self.weather.configure()

    def validate(self) -> None:
        """Validate the configured OTA, partition, weather, and web application components."""
        if self.ota:
            self.ota.validate()
        self.partition.validate()
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
        """Remove generated files and directories from the project workspace."""
        Deprecated.clean()
        Extra.clean()
        WebApp.clean()
        Tools.clean()
        for path in (
            "logs",
            "scripts/__pycache__",
        ):
            if os.path.exists(path):
                shutil.rmtree(path, ignore_errors=True)
                print(f"Removing {path}")
