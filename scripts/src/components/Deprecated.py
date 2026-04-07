import logging
import os
import pathlib
import typing

from ..modes.Clock import Clock
from ..modes.GameOfLife import GameOfLife
from ..modes.PingPong import PingPong
from ..modes.Snake import Snake
from ..modes.Stream import Stream
from ..modes.Weather import Weather

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens


class Deprecated:
    MIGRATIONS: list[tuple[str, str, str, str]] = [
        ("MODE_ARTNET", "Art-Net", Stream.ENV_OPTION, Stream.NAME),
        ("MODE_BOLDCLOCK", "Bold clock", Clock.ENV_OPTION, Clock.NAME),
        ("MODE_DISTRIBUTEDDISPLAYPROTOCOL", "Distributed Display Protocol", Stream.ENV_OPTION, Stream.NAME),
        ("MODE_E131", "E1.31", Stream.ENV_OPTION, Stream.NAME),
        ("MODE_GAMEOFLIFECLOCK", "Game of Life clock", GameOfLife.ENV_OPTION, GameOfLife.NAME),
        ("MODE_GOOGLEWEATHER", "Google weather", Weather.ENV_OPTION, Weather.NAME),
        ("MODE_HOMEASSISTANTWEATHER", "Home Assistant weather", Weather.ENV_OPTION, Weather.NAME),
        ("MODE_LARGECLOCK", "Large clock", Clock.ENV_OPTION, Clock.NAME),
        ("MODE_LARGETICKINGCLOCK", "Large ticking clock", Clock.ENV_OPTION, Clock.NAME),
        ("MODE_OPENMETEO", "Open-Meteo", Weather.ENV_OPTION, Weather.NAME),
        ("MODE_OPENWEATHER", "Open Weather", Weather.ENV_OPTION, Weather.NAME),
        ("MODE_PINGPONGCLOCK", "Ping-Pong clock", PingPong.ENV_OPTION, PingPong.NAME),
        ("MODE_SMALLCLOCK", "Small clock", Clock.ENV_OPTION, Clock.NAME),
        ("MODE_SMALLTICKINGCLOCK", "Small ticking clock", Clock.ENV_OPTION, Clock.NAME),
        ("MODE_SNAKECLOCK", "Snake clock", Snake.ENV_OPTION, Snake.NAME),
        ("MODE_WORLDWEATHERONLINE", "World Weather Online", Weather.ENV_OPTION, Weather.NAME),
        ("MODE_WTTRIN", "Wttr.in", Weather.ENV_OPTION, Weather.NAME),
        ("MODE_YR", "Yr", Weather.ENV_OPTION, Weather.NAME),
    ]
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        self.project = project

    def initialize(self) -> None:
        self._env()
        self._platformio_ini()

    def _env(self) -> None:
        for old_option, old_name, new_option, new_name in self.MIGRATIONS:
            if old_option in self.project.dotenv:
                if new_option == Weather.ENV_OPTION:
                    weather_option = f"WEATHER_{old_option.removeprefix('MODE_')}"
                    if old_name != "Open Weather":
                        weather_option = weather_option.removesuffix("WEATHER")
                    logging.warning(
                        "%s '%s' is deprecated. Use %s '%s' and %s '%s' instead.",
                        old_option,
                        old_name,
                        new_option,
                        new_name,
                        weather_option,
                        old_name,
                    )
                    if weather_option not in self.project.dotenv:
                        self.project.dotenv[weather_option] = self.project.dotenv[old_option]
                else:
                    logging.warning(
                        "%s '%s' is deprecated. Use %s '%s' instead.", old_option, old_name, new_option, new_name
                    )
                if new_option not in self.project.dotenv:
                    self.project.dotenv[new_option] = self.project.dotenv[old_option]
                del self.project.dotenv[old_option]

    def _platformio_ini(self) -> None:
        option = "board_build.embed_files"
        path = "firmware/embed/x509_crt_bundle.bin"
        paths = self.project.env.GetProjectOption(option, None)
        if paths and ((isinstance(paths, list) and path in paths) or (isinstance(paths, str) and paths == path)):
            logging.warning("'%s = %s' is deprecated and should be removed from platformio.ini.", option, path)
            _path = pathlib.Path(path)
            if not _path.exists():
                _path.parent.mkdir(parents=True, exist_ok=True)
                _path.write_bytes(b"\x00")

    @staticmethod
    def clean() -> None:
        for file in [
            "firmware/certs/bundle/ca_roots.pem",
            "firmware/embed/x509_crt_bundle.bin",
        ]:
            if os.path.isfile(file):
                os.remove(file)
                print(f"Removing {file}")
        for directory in [
            "firmware/certs/bundle",
            "firmware/certs",
            "firmware/embed",
        ]:
            if os.path.isdir(directory):
                os.rmdir(directory)
                print(f"Removing {directory}")
