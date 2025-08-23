import dotenv
import json
import os
import shutil


class Tools:
    def __init__(self, env) -> None:
        self.env = env

    def clean(self) -> None:
        for path in [
            "tools/__pycache__",
            "tools/src/__pycache__",
            "tools/src/config/__pycache__",
            "tools/src/frekvens.egg-info",
            "tools/src/modes/__pycache__",
            "tools/src/services/__pycache__",
        ]:
            if os.path.exists(path):
                shutil.rmtree(path, ignore_errors=True)
                print(f"Removing {path}")

    def environment(self) -> None:
        env_py_path = "tools/.env"
        env_pio = dotenv.dotenv_values(".env")
        env_py = dotenv.dotenv_values(env_py_path)
        wrote = False
        for option in [
            "DOMAIN",
            "HOSTNAME",
            "NAME",
        ]:
            if option in env_pio.keys() and env_pio.get(option) != env_py.get(option):
                dotenv.set_key(env_py_path, option, env_pio.get(option) or "")
                print(option)
                wrote = True
            elif option not in env_pio.keys() and option in env_py.keys():
                dotenv.unset_key(env_py_path, option)
                wrote = True
        for prefix in [
            "ENV",
            "EXTENSION",
            "FONT",
            "MODE",
        ]:
            for option, value in env_pio.items():
                if option.startswith(f"{prefix}_") and env_py.get(prefix) != value:
                    dotenv.set_key(env_py_path, option, value or "")
                    wrote = True
            for option, value in env_py.items():
                if (
                    option.startswith(f"{prefix}_")
                    and not option in env_pio.keys()
                ):
                    dotenv.unset_key(env_py_path, option)
                    wrote = True
        if wrote:
            print(env_py_path)

    def version(self) -> None:
        config = "tools/pyproject.toml"
        with open("library.json") as pio, open(config, "r+") as toml:
            library = json.load(pio)
            if "version" in library:
                lines = toml.readlines()
                for i, line in enumerate(lines):
                    if (
                        line.startswith("version = ")
                        and line != f'version = "{library["version"]}"\n'
                    ):
                        lines[i] = f'version = "{library["version"]}"\n'
                        toml.seek(0)
                        toml.writelines(lines)
                        print(config)
                        break
