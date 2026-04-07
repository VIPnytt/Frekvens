import decimal
import logging
import numbers
import pathlib
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
        count_weather = 0
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
                if value == "true":
                    if option.startswith("WEATHER_"):
                        count_weather += 1
            else:
                self.project.env.Append(
                    CPPDEFINES=[
                        (option, self.project.env.StringifyMacro(value)),
                    ]
                )
                if option == "TEMPERATURE_UNIT":
                    if value == "°C":
                        self.project.env.Append(
                            CPPDEFINES=[
                                ("TEMPERATURE_CELSIUS", "true"),
                            ]
                        )
                    elif value == "°F":
                        self.project.env.Append(
                            CPPDEFINES=[
                                ("TEMPERATURE_FAHRENHEIT", "true"),
                            ]
                        )
                    elif value == "°K":
                        self.project.env.Append(
                            CPPDEFINES=[
                                ("TEMPERATURE_KELVIN", "true"),
                            ]
                        )
                    else:
                        logging.warning(
                            "%s '%s' is unsupported. Valid values are '°C', '°F' and '°K'.",
                            option,
                            value,
                        )
        self.project.env.Append(
            CPPDEFINES=list(
                {
                    option: value
                    for option, value in {
                        "COUNT_WEATHER": count_weather,
                    }.items()
                    if value
                }.items()
            )
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
