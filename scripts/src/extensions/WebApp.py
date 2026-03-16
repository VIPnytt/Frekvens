import dotenv
import gzip
import nodejs_wheel
import os
import pathlib
import shutil
import subprocess
import typing
import warnings

from .WebSocket import WebSocket

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens


class WebApp:
    ENV_OPTION: str = "EXTENSION_WEBAPP"
    NAME: str = "Web app"
    path = pathlib.Path("webapp")
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        self.project = project

    def validate(self) -> None:
        if self.ENV_OPTION not in self.project.dotenv or self.project.dotenv[self.ENV_OPTION] == "false":
            self.project.webapp = None
        elif "no_fs" in str(self.project.partition.table):
            if self.ENV_OPTION in self.project.dotenv and self.project.dotenv[self.ENV_OPTION] == "true":
                warnings.warn(
                    f"{self.ENV_OPTION}: Partition table has no filesystem support.",
                    UserWarning,
                )
        elif WebSocket.ENV_OPTION not in self.project.dotenv or self.project.dotenv[WebSocket.ENV_OPTION] == "false":
            warnings.warn(
                f"{WebSocket.ENV_OPTION}: {WebSocket.NAME} is required by {self.NAME}.",
                UserWarning,
            )

    def finalize(self) -> None:
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
                dotenv.set_key(self.path / ".env", f"VITE_{option}", "true")
            elif option in options or option.startswith(prefixes):
                dotenv.set_key(self.path / ".env", f"VITE_{option}", value)
        config = self.project.env.GetProjectConfig()
        for option in [
            "board",
        ]:
            value = config.get(f"env:{self.project.env['PIOENV']}", option, None)
            if value:
                dotenv.set_key(self.path / ".env", f"VITE_{option.upper()}", value)
        for option in dotenv.dotenv_values(self.path / ".env").keys():
            if option.startswith("VITE_"):
                _option = option.removeprefix("VITE_")
                if _option not in self.project.dotenv and (_option in options or _option.startswith(prefixes)):
                    dotenv.unset_key(self.path / ".env", option)
        self._npm_build()

    def _npm_build(self) -> None:
        build_cmd = ["run", "build"]
        install_cmd = ["install"]
        try:
            if nodejs_wheel.npm(build_cmd, stderr=subprocess.DEVNULL, cwd=self.path):
                nodejs_wheel.npm(install_cmd, cwd=self.path, check=True)
                nodejs_wheel.npm(build_cmd, cwd=self.path, check=True)
        except FileNotFoundError:
            if subprocess.run(["npm", *build_cmd], stderr=subprocess.DEVNULL, cwd=self.path).returncode:
                subprocess.run(["npm", *install_cmd], cwd=self.path, check=True)
                subprocess.run(["npm", *build_cmd], cwd=self.path, check=True)
        data_dir = pathlib.Path("data") / "webapp"
        data_dir.mkdir(parents=True, exist_ok=True)
        with gzip.open(data_dir / "index.html.gz", "wb") as gz:
            gz.write((self.path / "dist" / "index.html").read_bytes())

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
