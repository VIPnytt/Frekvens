import dotenv
import json
import re


class Tools:
    def __init__(self, env):
        self.env = env

    def environment(self):
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
            "MODE",
        ]:
            for option, value in env_py.items():
                if (
                    re.fullmatch(rf"^{prefix}_[A-Z0-9]+$", option)
                    and not option in env_pio.keys()
                ):
                    dotenv.unset_key(env_py_path, option)
                    wrote = True
        if wrote:
            print(env_py_path)

    def version(self):
        with open("library.json") as pio, open("tools/project.toml", "r+") as toml:
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
                        print("tools/project.toml")
                        break
