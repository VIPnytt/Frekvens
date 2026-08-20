import decimal
import logging
import numbers
import pathlib
import typing

if typing.TYPE_CHECKING:
    from .components.Types import COMMAND_LINE_TARGETS, Environment
    from .Frekvens import Frekvens
else:
    from SCons.Script import COMMAND_LINE_TARGETS, Environment  # ruff:ignore[F401]


class Firmware:
    NAME: typing.Final[str] = "Firmware"
    path: pathlib.Path
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        self.path = pathlib.Path("firmware")
        self.project = project

    def initialize(self) -> None:
        if COMMAND_LINE_TARGETS in [
            ["buildfs"],
            ["uploadfs"],
            ["uploadfsota"],
        ]:
            self.project.firmware = None

    def finalize(self) -> None:
        self._define_env()
        self._define_pio()

    def _define_env(self) -> None:
        count_extension = 0
        for option, _value in self.project.dotenv.items():
            if (value := _value or "") in (
                "false",
                "true",
            ):
                self.project.env.Append(
                    CPPDEFINES=[
                        (option, "true" if value == "true" else "false"),
                    ]
                )
                if value == "true" and option.startswith("EXTENSION_"):
                    count_extension += 1
            else:
                self.project.env.Append(
                    CPPDEFINES=[
                        (option, self.project.env.StringifyMacro(value)),
                    ]
                )
                if option == "TEMPERATURE_UNIT":
                    _option = {
                        "°C": "TEMPERATURE_CELSIUS",
                        "°F": "TEMPERATURE_FAHRENHEIT",
                        "°K": "TEMPERATURE_KELVIN",
                    }.get(value)
                    if _option is None:
                        logging.warning("%s %r is unsupported. Valid values are '°C', '°F' and '°K'.", option, value)
                    else:
                        self.project.env.Append(
                            CPPDEFINES=[
                                (_option, "true"),
                            ]
                        )
        self.project.env.Append(
            CPPDEFINES=list(
                {
                    option: value
                    for option, value in {
                        "COUNT_EXTENSION": count_extension,
                    }.items()
                    if value
                }.items()
            )
        )

    def _define_pio(self) -> None:
        config = self.project.env.GetProjectConfig()
        for option in (
            "board",
            "monitor_speed",
        ):
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
