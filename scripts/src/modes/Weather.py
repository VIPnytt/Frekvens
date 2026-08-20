import logging
import typing
import urllib.parse

if typing.TYPE_CHECKING:
    from ..components.Types import COMMAND_LINE_TARGETS
    from ..Frekvens import Frekvens
else:
    from SCons.Script import COMMAND_LINE_TARGETS


class Weather:
    ENV_OPTION: typing.Final[str] = "MODE_WEATHER"
    NAME: typing.Final[str] = "Weather"

    def __init__(self, project: "Frekvens") -> None:
        """Initialize the weather mode with a project reference."""
        self.project = project

    def initialize(self) -> None:
        """Disable weather configuration for filesystem build and upload targets."""
        if COMMAND_LINE_TARGETS in [
            ["buildfs"],
            ["uploadfs"],
            ["uploadfsota"],
        ]:
            self.project.weather = None

    def configure(self) -> None:
        """
        Configure weather settings and normalize location-related options.

        Weather is disabled unless the weather mode option is set to ``"true"``.
        Latitude and longitude are formatted as decimal strings with up to four
        decimal places, and the location is URL-encoded when provided.
        """
        if self.ENV_OPTION not in self.project.dotenv or self.project.dotenv[self.ENV_OPTION] != "true":
            self.project.weather = None
            return
        for option in (
            "LATITUDE",
            "LONGITUDE",
        ):
            if option in self.project.dotenv:
                self.project.dotenv[option] = f"{float(self.project.dotenv[option]):.4f}".rstrip("0").rstrip(".")
        if "LOCATION" in self.project.dotenv:
            self.project.dotenv["LOCATION"] = urllib.parse.quote(self.project.dotenv["LOCATION"])

    def validate(self) -> None:
        found = False
        for option, value in self.project.dotenv.items():
            if value == "true" and option.startswith("WEATHER_"):
                found = True
                break
        if not found:
            logging.error("%s: at least one provider is required.", self.ENV_OPTION)
