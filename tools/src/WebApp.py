import dotenv
import gzip
import json
import os
import pathlib
import re
import subprocess
import sys


class WebApp:
    def __init__(self, env):
        self.env = env
        self.check()

    def build(self):
        with open("library.json") as pio, open("webapp/package.json") as npm, open(
            "webapp/package-lock.json"
        ) as npm_lock:
            library = json.load(pio)
            package = json.load(npm)
            package_lock = json.load(npm_lock)
            if (
                "version" not in package_lock
                or library["version"] != package["version"]
                or library["version"] != package_lock["version"]
            ):
                self.env.Execute(f"cd webapp && npm install && npm run build")
            else:
                if self.env.Execute(f"cd webapp && npm run build"):
                    self.env.Execute(f"cd webapp && npm install && npm run build")
            prefix = "data/webapp"
            pathlib.Path(prefix).mkdir(parents=True, exist_ok=True)
            index = f"{prefix}/v{library['version']}.html.gz"
            with open("webapp/dist/index.html", "rb") as html:
                with gzip.open(index, "wb") as gz:
                    gz.writelines(html)
                    print(index)

    def check(self):
        try:
            subprocess.run(
                ["node", "--version"],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                check=True,
            )
        except (subprocess.CalledProcessError, FileNotFoundError):
            print("Node.js is required but was not found on your system.")
            print("Please install Node.js from https://nodejs.org/")
            sys.exit(1)

    def clean(self):
        for data in os.scandir("data/webapp"):
            if data.is_file():
                pathlib.Path(data.path).unlink()
        for dist in os.scandir("webapp/dist"):
            if dist.is_file():
                pathlib.Path(dist.path).unlink()

    def evironment(self):
        env_ts_path = "webapp/.env"
        env_pio = dotenv.dotenv_values(".env")
        env_ts = dotenv.dotenv_values(env_ts_path)
        wrote = False
        for option in [
            "NAME",
        ]:
            if option in env_pio.keys() and env_pio.get(option) != env_ts.get(
                f"VITE_{option}"
            ):
                dotenv.set_key(env_ts_path, f"VITE_{option}", env_pio.get(option) or "")
                wrote = True
            elif option not in env_pio.keys() and f"VITE_{option}" in env_ts.keys():
                dotenv.unset_key(env_ts_path, f"VITE_{option}")
                wrote = True
        for prefix in [
            "ENV",
            "EXTENSION",
            "MODE",
        ]:
            for option, value in env_ts.items():
                if (
                    re.fullmatch(rf"^VITE_{prefix}_[A-Z0-9]+$", option)
                    and not option.removeprefix("VITE_") in env_pio.keys()
                ):
                    dotenv.unset_key(env_ts_path, option)
                    wrote = True
        if wrote:
            print(env_ts_path)

    def environment_dev(self):
        env_ts_path = "webapp/.env.development"
        env_pio = dotenv.dotenv_values(".env")
        env_ts = dotenv.dotenv_values(env_ts_path)
        for option, value in env_pio.items():
            if option.upper() == "HOSTNAME":
                host = (value or "frekvens") + (env_pio.get("DOMAIN") or ".local")
                if env_ts.get("VITE_HOST") != host:
                    dotenv.set_key(env_ts_path, "VITE_HOST", host)
                    print(env_ts_path)

    def version(self):
        package_json_path = "webapp/package.json"
        with open("library.json") as pio, open(package_json_path) as npm:
            library = json.load(pio)
            if "version" in library:
                package = json.load(npm)
                if "version" not in package or package["version"] != library["version"]:
                    if not self.env.Execute(
                        f"cd webapp && npm version {library['version']}"
                    ):
                        print(package_json_path)
