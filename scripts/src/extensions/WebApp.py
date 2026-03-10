import dotenv
import gzip
import json
import os
import pathlib
import re
import shutil
import subprocess
import sys
import typing
import warnings

from ..config.version import VERSION
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

    def initialize(self) -> None:
        with open(self.path / "package.json", encoding="utf-8") as f:
            package = json.load(f)
            if package["version"] != VERSION:
                raise ValueError(f"{self.NAME} version mismatch")
        with open(self.path / "package-lock.json", encoding="utf-8") as f_lock:
            lock = json.load(f_lock)
            if lock["version"] != VERSION or lock["packages"][""]["version"] != VERSION:
                raise ValueError(f"{self.NAME} version mismatch")

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

    def validate(self) -> None:
        if self.ENV_OPTION not in self.project.dotenv or self.project.dotenv[self.ENV_OPTION] == "false":
            self.project.webapp = None
        elif "no_fs" in self.project.partition.table:
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

    def _npm_build(self) -> None:
        node_dir = (pathlib.Path(".nodeenv") / ("Scripts" if os.name == "nt" else "bin")).resolve()
        npm = shutil.which("npm")
        if not npm:
            npm = shutil.which("npm", path=node_dir)
            if not npm:
                subprocess.run([sys.executable, "-m", "nodeenv", f"--node={self._node_version() or 'lts'}", ".nodeenv"])
                npm = shutil.which("npm", path=node_dir) or "npm"
        env = os.environ.copy()
        if npm != "npm" and pathlib.Path(npm).parent == node_dir:
            env["PATH"] = f"{node_dir}{os.pathsep}{env['PATH']}"
        build = subprocess.run([npm, "run", "build"], cwd=self.path, env=env, stderr=subprocess.DEVNULL)
        if build.returncode:
            subprocess.run([npm, "install"], check=True, cwd=self.path, env=env)
            subprocess.run(build.args, check=True, cwd=self.path, env=env)
        data_dir = pathlib.Path("data") / "webapp"
        data_dir.mkdir(parents=True, exist_ok=True)
        with open(self.path / "dist" / "index.html", "rb") as html:
            with gzip.open(data_dir / "index.html.gz", "wb") as gz:
                shutil.copyfileobj(html, gz)

    def _node_version(self) -> str | None:
        with open(self.path / "package.json", encoding="utf-8") as f:
            package = json.load(f)
        match = re.search(r"\d+", package["engines"]["node"])
        if match:
            prefix = f"{match.group()}."
            for version in reversed(
                subprocess.run(
                    [sys.executable, "-m", "nodeenv", "--list"],
                    capture_output=True,
                    text=True,
                ).stderr.split()
            ):
                if version.startswith(prefix):
                    return version
        return None

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
