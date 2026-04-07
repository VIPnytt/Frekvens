import dotenv
import os
import pathlib
import shutil
import typing

from .components.Dependency import Dependency
from .components.Deprecated import Deprecated
from .components.Partition import Partition
from .components.TimeZone import TimeZone
from .config.version import VERSION
from .extensions.Ota import Ota
from .extensions.WebApp import WebApp
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
    timezone: TimeZone | None = None
    tools: Tools
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
        if COMMAND_LINE_TARGETS not in [
            ["buildfs"],
            ["uploadfs"],
            ["uploadfsota"],
        ]:
            self.timezone = TimeZone(self)
        if COMMAND_LINE_TARGETS not in [
            ["build"],
            ["compiledb"],
            ["upload"],
        ]:
            self.webapp = WebApp(self)
        self.dotenv = {key: (value if value is not None else "") for key, value in dotenv.dotenv_values(".env").items()}

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
        if self.timezone:
            self.timezone.configure()

    def validate(self) -> None:
        if self.ota:
            self.ota.validate()
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
        for path in [
            "logs",
            "scripts/__pycache__",
        ]:
            if os.path.exists(path):
                shutil.rmtree(path, ignore_errors=True)
                print(f"Removing {path}")
