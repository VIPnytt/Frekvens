import decimal
import dotenv
import hashlib
import json
import numbers
import os
import tzlocal

from tools.src.config import ikeaFrekvens, ikeaObegransad
from tools.src.services import Connectivity


class Firmware:
    def __init__(self, env) -> None:
        self.env = env

    def define(self) -> None:
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
        iana = env_pio.get("TIME_ZONE_IANA")
        if iana is None:
            iana = tzlocal.get_localzone_name()
        else:
            self.env.Append(
                CPPDEFINES=[
                    (
                        "TIME_ZONE_IANA",
                        self.env.StringifyMacro(iana),
                    ),
                ]
            )
        if iana:
            posix = Connectivity.Connectivity().tz_lookup(iana)
            if posix:
                self.env.Append(
                    CPPDEFINES=[
                        (
                            "TIME_ZONE_POSIX",
                            self.env.StringifyMacro(posix),
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

    def upload(self) -> None:
        env_pio = dotenv.dotenv_values(".env")
        for option, value in {
            "ENV_FREKVENS": ikeaFrekvens.HOSTNAME,
            "ENV_OBEGRANSAD": ikeaObegransad.HOSTNAME,
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

    def version(self) -> None:
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
