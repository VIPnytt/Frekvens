import decimal
import numbers
import os
import pathlib
import re
import typing

if typing.TYPE_CHECKING:
    from .Frekvens import Frekvens


class Firmware:
    NAME: str = "Firmware"
    path = pathlib.Path("firmware")
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        self.project = project

    def finalize(self) -> None:
        self._define_env()
        self._define_pio()

    def _define_env(self) -> None:
        for option, _value in self.project.dotenv.items():
            if (value := _value or "") in [
                "false",
                "true",
            ]:
                self.project.env.Append(
                    CPPDEFINES=[
                        (option, "true" if value == "true" else "false"),
                    ]
                )
            else:
                self.project.env.Append(
                    CPPDEFINES=[
                        (option, self.project.env.StringifyMacro(value)),
                    ]
                )

    def _define_pio(self) -> None:
        config = self.project.env.GetProjectConfig()
        for option in [
            "board",
            "monitor_speed",
        ]:
            if value := config.get(f"env:{self.project.env['PIOENV']}", option, None):
                _value = (
                    value
                    if isinstance(value, (decimal.Decimal, numbers.Number))
                    else self.project.env.StringifyMacro(value)
                )
                self.project.env.Append(
                    CPPDEFINES=[
                        (option.replace(".", "__").upper(), _value),
                    ]
                )
        for option in [
            "board_build.embed_files",
            "board_build.embed_txtfiles",
        ]:
            if embed_files := self.project.env.GetProjectOption(option, None):
                if not isinstance(embed_files, list):
                    embed_files = [embed_files]
                _prefix = option.replace(".", "__").upper()
                for embed_file in embed_files:
                    _key = (
                        _prefix
                        + "__"
                        + re.sub(
                            r"_+",
                            "_",
                            re.sub(
                                r"[^A-Za-z0-9]",
                                "_",
                                pathlib.Path(embed_file).stem.upper(),
                            ),
                        ).strip("_")
                    )
                    self.project.env.Append(
                        CPPDEFINES=[
                            (_key, self.project.env.StringifyMacro(re.sub(r"[^A-Za-z0-9]", "_", embed_file))),
                        ]
                    )

    @staticmethod
    def clean() -> None:
        for file in [
            "firmware/certs/bundle/ca_roots.pem",
            "firmware/embed/x509_crt_bundle.bin",
        ]:
            if os.path.isfile(file):
                os.remove(file)
                print(f"Removing {file}")
