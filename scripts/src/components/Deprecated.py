import logging
import typing

from ..modes.Clock import Clock
from ..modes.GameOfLife import GameOfLife
from ..modes.PingPong import PingPong
from ..modes.Snake import Snake
from ..modes.Stream import Stream

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens


class Deprecated:
    MIGRATIONS: list[tuple[str, str, str, str]] = [
        ("MODE_ARTNET", "Art-Net", Stream.ENV_OPTION, Stream.NAME),
        ("MODE_BOLDCLOCK", "Bold clock", Clock.ENV_OPTION, Clock.NAME),
        ("MODE_DISTRIBUTEDDISPLAYPROTOCOL", "Distributed Display Protocol", Stream.ENV_OPTION, Stream.NAME),
        ("MODE_E131", "E1.31", Stream.ENV_OPTION, Stream.NAME),
        ("MODE_GAMEOFLIFECLOCK", "Game of Life clock", GameOfLife.ENV_OPTION, GameOfLife.NAME),
        ("MODE_LARGECLOCK", "Large clock", Clock.ENV_OPTION, Clock.NAME),
        ("MODE_LARGETICKINGCLOCK", "Large ticking clock", Clock.ENV_OPTION, Clock.NAME),
        ("MODE_PINGPONGCLOCK", "Ping-Pong clock", PingPong.ENV_OPTION, PingPong.NAME),
        ("MODE_SMALLCLOCK", "Small clock", Clock.ENV_OPTION, Clock.NAME),
        ("MODE_SMALLTICKINGCLOCK", "Small ticking clock", Clock.ENV_OPTION, Clock.NAME),
        ("MODE_SNAKECLOCK", "Snake clock", Snake.ENV_OPTION, Snake.NAME),
    ]
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        self.project = project

    def initialize(self) -> None:
        for old_option, old_name, new_option, new_name in self.MIGRATIONS:
            if old_option in self.project.dotenv:
                logging.warning(f"{old_option} '{old_name}' is deprecated. Use {new_option} '{new_name}' instead.")
                if new_option not in self.project.dotenv:
                    self.project.dotenv[new_option] = self.project.dotenv[old_option]
                del self.project.dotenv[old_option]
