import logging
import typing
import urllib.parse

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens


class Weather:
    ENV_OPTION: str = "MODE_WEATHER"
    NAME: str = "Weather"

    def __init__(self, project: "Frekvens") -> None:
        self.project = project

    def configure(self) -> None:
        if self.ENV_OPTION not in self.project.dotenv or self.project.dotenv[self.ENV_OPTION] != "true":
            self.project.weather = None
            return
        for option in {
            "LATITUDE",
            "LONGITUDE",
        }:
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
