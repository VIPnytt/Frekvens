import gzip
import logging
import os
import pathlib
import shutil
import subprocess
import typing

import dotenv
import nodejs_wheel

from .WebSocket import WebSocket

if typing.TYPE_CHECKING:
    from ..components.Types import COMMAND_LINE_TARGETS, Environment
    from ..Frekvens import Frekvens
else:
    from SCons.Script import COMMAND_LINE_TARGETS, Environment


class WebApp:
    ENV_OPTION: typing.Final[str] = "EXTENSION_WEBAPP"
    NAME: typing.Final[str] = "Web app"
    path: pathlib.Path
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        """Initialize the web application extension for a project.
        
        Parameters:
        	project (Frekvens): The project associated with the web application.
        """
        self.path = pathlib.Path("webapp")
        self.project = project

    def initialize(self) -> None:
        """
        Disable the web app for command-line targets that do not require it.
        """
        if COMMAND_LINE_TARGETS in [
            ["build"],
            ["compiledb"],
            ["upload"],
        ]:
            self.project.webapp = None

    def validate(self) -> None:
        """Validate web app configuration against the project settings and partition table."""
        if self.ENV_OPTION not in self.project.dotenv or self.project.dotenv[self.ENV_OPTION] == "false":
            self.project.webapp = None
        elif "no_fs" in self.project.partition.table.name:
            if self.ENV_OPTION in self.project.dotenv and self.project.dotenv[self.ENV_OPTION] == "true":
                logging.error("%s: Partition table has no filesystem support.", self.ENV_OPTION)
        elif WebSocket.ENV_OPTION not in self.project.dotenv or self.project.dotenv[WebSocket.ENV_OPTION] == "false":
            logging.warning("%s: %s is required by %s.", WebSocket.ENV_OPTION, WebSocket.NAME, self.NAME)

    def finalize(self) -> None:
        """
        Synchronize project settings with the web app, build the app, and register OTA commissioning actions.
        """
        options = (
            "HOSTNAME",
            "NAME",
            "TEMPERATURE_UNIT",
        )
        prefixes = (
            "EXTENSION_",
            "FONT_",
            "IKEA_",
            "MODE_",
        )
        for option, value in self.project.dotenv.items():
            if option in ("OTA_KEY",):
                dotenv.set_key(self.path / ".env", f"VITE_{option}", "true")
            elif option in options or option.startswith(prefixes):
                dotenv.set_key(self.path / ".env", f"VITE_{option}", value)
        config = self.project.env.GetProjectConfig()
        for option in ("board",):
            value = config.get(f"env:{self.project.env['PIOENV']}", option, None)
            if value:
                dotenv.set_key(self.path / ".env", f"VITE_{option.upper()}", value)
        for option in dotenv.dotenv_values(self.path / ".env"):
            if option.startswith("VITE_"):
                _option = option.removeprefix("VITE_")
                if _option not in self.project.dotenv and (_option in options or _option.startswith(prefixes)):
                    dotenv.unset_key(self.path / ".env", option)
        self._npm_build()
        for target in (
            "uploadfsota",
            "uploadota",
        ):
            self.project.env.AddPostAction(target, self._commissioning)

    def _commissioning(self, target: list[str], source: list[str], env: Environment) -> None:
        """
        Display the local URL for the web application.
        
        Parameters:
        	target (list[str]): Commissioning target paths.
        	source (list[str]): Commissioning source paths.
        	env (Environment): Build environment associated with the commissioning operation.
        """
        print(f"{self.NAME}: http://{self.project.dotenv['HOSTNAME']}.local")

    def _npm_build(self) -> None:
        """
        Build the web application and compress its generated index page for packaging.
        
        The build retries after installing dependencies when the initial build fails and falls back to the system `npm` executable when the bundled npm tool is unavailable.
        """
        build_cmd = ["run", "build"]
        install_cmd = ["install"]
        try:
            if nodejs_wheel.npm(build_cmd, stderr=subprocess.DEVNULL, cwd=self.path, check=False):
                nodejs_wheel.npm(install_cmd, cwd=self.path, check=True)
                nodejs_wheel.npm(build_cmd, cwd=self.path, check=True)
        except FileNotFoundError:
            if subprocess.run(["npm", *build_cmd], stderr=subprocess.DEVNULL, cwd=self.path, check=False).returncode:
                subprocess.run(["npm", *install_cmd], cwd=self.path, check=True)
                subprocess.run(["npm", *build_cmd], cwd=self.path, check=True)
        data_dir = pathlib.Path("data") / "webapp"
        data_dir.mkdir(parents=True, exist_ok=True)
        with gzip.open(data_dir / "index.html.gz", "wb") as gz:
            gz.write((self.path / "dist" / "index.html").read_bytes())

    @staticmethod
    def clean() -> None:
        """Remove generated web app files and dependencies from the project."""
        for path in (
            "data/webapp",
            "webapp/dist",
        ):
            for data in os.scandir(path):
                if data.is_file():
                    pathlib.Path(data.path).unlink()
                    print(f"Removing {data.path}")
        node_modules = "webapp/node_modules"
        if os.path.exists(node_modules):
            shutil.rmtree(node_modules, ignore_errors=True)
            print(f"Removing {node_modules}")
