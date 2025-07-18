import decimal
import dotenv
import hashlib
import json
import numbers
import os

from tools.src.config import Frekvens
from tools.src.config import Obegransad


class Firmware:
    def __init__(self, env):
        self.env = env

    def define(self):
        env_pio = dotenv.dotenv_values(".env")
        for option, value in env_pio.items():
            if option.upper() in [
                "ENV_FREKVENS",
                "ENV_OBEGRANSAD",
            ]:
                self.env.Append(
                    CPPDEFINES=[
                        (
                            option.upper(),
                            1,
                        ),
                    ]
                )
            elif option.upper() == "OTA_KEY":
                self.env.Append(
                    CPPDEFINES=[
                        (
                            option.upper(),
                            self.env.StringifyMacro(value),
                        ),
                        (
                            "OTA_KEY_HASH",
                            self.env.StringifyMacro(
                                hashlib.md5((value or "").encode()).hexdigest()
                            ),
                        ),
                    ]
                )
            elif option.upper() == "TIME_ZONE_IANA":
                with open(
                    f"{self.env['PROJECT_LIBDEPS_DIR']}\\{self.env['PIOENV']}\\posix_tz_db\\zones.json"
                ) as zones:
                    iana = json.load(zones)
                    if value in iana:
                        self.env.Append(
                            CPPDEFINES=[
                                (
                                    option.upper(),
                                    self.env.StringifyMacro(value),
                                ),
                                (
                                    "TIME_ZONE_POSIX",
                                    self.env.StringifyMacro(iana[value]),
                                ),
                            ]
                        )
            elif value is not None and value.lower() in [
                "true",
                "false",
            ]:
                self.env.Append(
                    CPPDEFINES=[
                        (
                            option.upper(),
                            (
                                "true"
                                if value is not None and value.lower() != "false"
                                else "false"
                            ),
                        ),
                    ]
                )
        self.env.Append(
            CPPDEFINES=[
                (
                    "BOARD_NAME",
                    self.env.StringifyMacro(self.env.BoardConfig().get("name")),
                ),
            ]
        )
        config = self.env.GetProjectConfig()
        section = f"env:{self.env['PIOENV']}"
        for option in [
            "board",
            "board_build.f_cpu",
            "board_build.f_flash",
            "board_build.flash_mode",
            "board_build.mcu",
            "board_build.partitions",
            "board_upload.flash_size",
            "board_upload.maximum_ram_size",
            "board_upload.maximum_size",
            "monitor_speed",
        ]:
            if config.has_option(section, option):
                value = config.get(section, option)
                define = option.replace(".", "__")
                flag = (
                    value
                    if isinstance(value, (decimal.Decimal, numbers.Number))
                    else self.env.StringifyMacro(value)
                )
                self.env.Append(
                    CPPDEFINES=[
                        (define.upper(), flag),
                    ]
                )

    def upload(self):
        env_pio = dotenv.dotenv_values(".env")
        for option, value in {
            "ENV_FREKVENS": Frekvens.HOSTNAME,
            "ENV_OBEGRANSAD": Obegransad.HOSTNAME,
        }.items():
            if (
                option in env_pio.keys()
                and not "HOSTNAME" in env_pio.keys()
                and not "HOSTNAME" in os.environ
            ):
                os.environ["HOSTNAME"] = value
                break
        for option, value in {
            "DOMAIN": ".local",
            "HOSTNAME": "frekvens",
            "OTA_KEY": "",
        }.items():
            if not option in os.environ and not option in env_pio.keys():
                os.environ[option] = value

    def version(self):
        config = "firmware/include/config/version.h"
        with open("library.json") as pio, open(config, "r+") as h:
            library = json.load(pio)
            if "version" in library:
                lines = h.readlines()
                for i, line in enumerate(lines):
                    if (
                        line.startswith('#define VERSION "')
                        and line != f'#define VERSION "{library["version"]}"\n'
                    ):
                        lines[i] = f'#define VERSION "{library["version"]}"\n'
                        h.seek(0)
                        h.writelines(lines)
                        print(config)
                        break
