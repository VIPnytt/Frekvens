import logging
import typing

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens


class Deprecated:
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        self.project = project

    def initialize(self) -> None:
        self._artnet()
        self._boldclock()
        self._distributeddisplayprotocol()
        self._e131()
        self._gameoflifeclock()
        self._largeclock()
        self._largetickingclock()
        self._pingpongclock()
        self._smallclock()
        self._smalltickingclock()
        self._snakeclock()

    def _artnet(self) -> None:
        self._migrate("MODE_ARTNET", "Art-Net", "MODE_STREAM", "Stream")

    def _boldclock(self) -> None:
        self._migrate_to_clock("MODE_BOLDCLOCK", "Bold clock")

    def _distributeddisplayprotocol(self) -> None:
        self._migrate("MODE_DISTRIBUTEDDISPLAYPROTOCOL", "Distributed Display Protocol", "MODE_STREAM", "Stream")

    def _e131(self) -> None:
        self._migrate("MODE_E131", "E1.31", "MODE_STREAM", "Stream")

    def _gameoflifeclock(self) -> None:
        self._migrate("MODE_GAMEOFLIFECLOCK", "Game of Life clock", "MODE_GAMEOFLIFE", "Game of Life")

    def _largeclock(self) -> None:
        self._migrate_to_clock("MODE_LARGECLOCK", "Large clock")

    def _largetickingclock(self) -> None:
        self._migrate_to_clock("MODE_LARGETICKINGCLOCK", "Large ticking clock")

    def _migrate(self, old_key: str, old_name: str, new_key: str, new_name: str) -> None:
        if old_key in self.project.dotenv:
            logging.warning(f'Deprecation: {old_key} "{old_name}" is deprecated. Use {new_key} "{new_name}" instead.')
            if new_key not in self.project.dotenv:
                self.project.dotenv[new_key] = self.project.dotenv[old_key]
            del self.project.dotenv[old_key]

    def _migrate_to_clock(self, old_key: str, old_name: str) -> None:
        self._migrate(old_key, old_name, "MODE_CLOCK", "Clock")

    def _pingpongclock(self) -> None:
        self._migrate("MODE_PINGPONGCLOCK", "Ping-Pong clock", "MODE_PINGPONG", "Ping-pong")

    def _smallclock(self) -> None:
        self._migrate_to_clock("MODE_SMALLCLOCK", "Small clock")

    def _smalltickingclock(self) -> None:
        self._migrate_to_clock("MODE_SMALLTICKINGCLOCK", "Small ticking clock")

    def _snakeclock(self) -> None:
        self._migrate("MODE_SNAKECLOCK", "Snake clock", "MODE_SNAKE", "Snake")
