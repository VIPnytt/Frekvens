import dotenv
import gzip
import json
import os
import pathlib
import shutil
import subprocess
import typing
import warnings

from ..config.version import VERSION
from .WebSocket import WebSocket

if typing.TYPE_CHECKING:
    from ..Project import Project


class WebApp:
    ENV_OPTION: str = "EXTENSION_WEBAPP"
    NAME: str = "Web app"
    project: "Project"

    def __init__(self, project: "Project") -> None:
        self.project = project

    def initialize(self) -> None:
        with open("webapp/package.json") as npm:
            package = json.load(npm)
            if package["version"] != VERSION:
                raise ValueError(f"{self.NAME} version mismatch")
        with open("webapp/package-lock.json") as npm_lock:
            lock = json.load(npm_lock)
            if lock["version"] != VERSION or lock["packages"][""]["version"] != VERSION:
                raise ValueError(f"{self.NAME} version mismatch")

    def finalize(self) -> None:
        path = "webapp/.env"
        options = [
            "HOSTNAME",
            "NAME",
        ]
        prefixes = (
            "EXTENSION_",
            "FONT_",
            "IKEA_",
            "MODE_",
        )
        for option, value in self.project.dotenv.items():
            if option in [
                "OTA_KEY",
            ]:
                dotenv.set_key(path, f"VITE_{option}", "true")
            elif option in options or option.startswith(prefixes):
                dotenv.set_key(path, f"VITE_{option}", value)
        config = self.project.env.GetProjectConfig()
        for option in [
            "board",
        ]:
            if config.has_option(self.project.working, option):
                dotenv.set_key(
                    path,
                    f"VITE_{option.upper()}",
                    config.get(self.project.working, option),
                )
        for option in dotenv.dotenv_values(path).keys():
            if option.startswith("VITE_"):
                _option = option.removeprefix("VITE_")
                if _option not in self.project.dotenv and (
                    _option in options or _option.startswith(prefixes)
                ):
                    dotenv.unset_key(path, option)
        self._npm_build()

    @staticmethod
    def clean() -> None:
        for path in [
            "data/webapp",
            "webapp/dist",
        ]:
            for data in os.scandir(path):
                if data.is_file():
                    pathlib.Path(data.path).unlink()
                    print(f"Removing {data.path}")
        node_modules = "webapp/node_modules"
        if os.path.exists(node_modules):
            shutil.rmtree(node_modules, ignore_errors=True)
            print(f"Removing {node_modules}")

    def validate(self) -> None:
        if (
            self.ENV_OPTION not in self.project.dotenv
            or self.project.dotenv[self.ENV_OPTION] == "false"
        ):
            self.project.webapp = None
        elif "no_fs" in self.project.partition.table:
            if (
                self.ENV_OPTION in self.project.dotenv
                and self.project.dotenv[self.ENV_OPTION] == "true"
            ):
                warnings.warn(
                    f"{self.ENV_OPTION}: Partition table has no filesystem support.",
                    UserWarning
                )
        elif (
            WebSocket.ENV_OPTION not in self.project.dotenv
            or self.project.dotenv[WebSocket.ENV_OPTION] == "false"
        ):
            warnings.warn(
                f"{WebSocket.ENV_OPTION}: {WebSocket.NAME} is required by {self.NAME}.",
                UserWarning
            )
        elif not self._node():
            raise Exception(
                "Node.js is required but was not found, please install. https://nodejs.org"
            )

    def _node(self) -> bool:
        try:
            subprocess.run(
                ["node", "--version"],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                check=True,
            )
            return True
        except (subprocess.CalledProcessError, FileNotFoundError):
            return False

    def _npm_build(self) -> None:
        if self.project.env.Execute(f"cd webapp && npm run build"):
            self.project.env.Execute(f"cd webapp && npm install && npm run build")
        prefix = "data/webapp"
        pathlib.Path(prefix).mkdir(parents=True, exist_ok=True)
        index = f"{prefix}/index.html.gz"
        with open("webapp/dist/index.html", "rb") as html:
            with gzip.open(index, "wb") as gz:
                gz.writelines(html)
