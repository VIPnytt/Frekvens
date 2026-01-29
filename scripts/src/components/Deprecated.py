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
        self._largetickingclock()
        self._smalltickingclock()
        self._snakeclock()

    def _artnet(self) -> None:
        if "MODE_ARTNET" in self.project.dotenv:
            logging.warning('Deprecation: MODE_ARTNET "Art-Net" is deprecated. Use MODE_STREAM "Stream" instead.')
            if "MODE_STREAM" not in self.project.dotenv:
                self.project.dotenv["MODE_STREAM"] = self.project.dotenv["MODE_ARTNET"]
            del self.project.dotenv["MODE_ARTNET"]

    def _boldclock(self) -> None:
        if "MODE_BOLDCLOCK" in self.project.dotenv:
            logging.warning(
                'Deprecation: MODE_BOLDCLOCK "Bold clock" is deprecated. Use MODE_LARGECLOCK "Large clock" instead.'
            )
            if "MODE_LARGECLOCK" not in self.project.dotenv:
                self.project.dotenv["MODE_LARGECLOCK"] = self.project.dotenv["MODE_BOLDCLOCK"]
            del self.project.dotenv["MODE_BOLDCLOCK"]

    def _distributeddisplayprotocol(self) -> None:
        if "MODE_DISTRIBUTEDDISPLAYPROTOCOL" in self.project.dotenv:
            logging.warning(
                'Deprecation: MODE_DISTRIBUTEDDISPLAYPROTOCOL "Distributed Display Protocol" is deprecated. Use MODE_STREAM "Stream" instead.'
            )
            if "MODE_STREAM" not in self.project.dotenv:
                self.project.dotenv["MODE_STREAM"] = self.project.dotenv["MODE_DISTRIBUTEDDISPLAYPROTOCOL"]
            del self.project.dotenv["MODE_DISTRIBUTEDDISPLAYPROTOCOL"]

    def _e131(self) -> None:
        if "MODE_E131" in self.project.dotenv:
            logging.warning('Deprecation: MODE_E131 "E1.31" is deprecated. Use MODE_STREAM "Stream" instead.')
            if "MODE_STREAM" not in self.project.dotenv:
                self.project.dotenv["MODE_STREAM"] = self.project.dotenv["MODE_E131"]
            del self.project.dotenv["MODE_E131"]

    def _gameoflifeclock(self) -> None:
        if "MODE_GAMEOFLIFECLOCK" in self.project.dotenv:
            logging.warning(
                'Deprecation: MODE_GAMEOFLIFECLOCK "Game of Life clock" is deprecated. Use MODE_GAMEOFLIFE "Game of Life" instead.'
            )
            if "MODE_GAMEOFLIFE" not in self.project.dotenv:
                self.project.dotenv["MODE_GAMEOFLIFE"] = self.project.dotenv["MODE_GAMEOFLIFECLOCK"]
            del self.project.dotenv["MODE_GAMEOFLIFECLOCK"]

    def _largetickingclock(self) -> None:
        if "MODE_LARGETICKINGCLOCK" in self.project.dotenv:
            logging.warning(
                'Deprecation: MODE_LARGETICKINGCLOCK "Large ticking clock" is deprecated. Use MODE_LARGECLOCK "Large clock" instead.'
            )
            if "MODE_LARGECLOCK" not in self.project.dotenv:
                self.project.dotenv["MODE_LARGECLOCK"] = self.project.dotenv["MODE_LARGETICKINGCLOCK"]
            del self.project.dotenv["MODE_LARGETICKINGCLOCK"]

    def _smalltickingclock(self) -> None:
        if "MODE_SMALLTICKINGCLOCK" in self.project.dotenv:
            logging.warning(
                'Deprecation: MODE_SMALLTICKINGCLOCK "Small ticking clock" is deprecated. Use MODE_SMALLCLOCK "Small clock" instead.'
            )
            if "MODE_SMALLCLOCK" not in self.project.dotenv:
                self.project.dotenv["MODE_SMALLCLOCK"] = self.project.dotenv["MODE_SMALLTICKINGCLOCK"]
            del self.project.dotenv["MODE_SMALLTICKINGCLOCK"]

    def _snakeclock(self) -> None:
        if "MODE_SNAKECLOCK" in self.project.dotenv:
            logging.warning('Deprecation: MODE_SNAKECLOCK "Snake clock" is deprecated. Use MODE_SNAKE "Snake" instead.')
            if "MODE_SNAKE" not in self.project.dotenv:
                self.project.dotenv["MODE_SNAKE"] = self.project.dotenv["MODE_SNAKECLOCK"]
            del self.project.dotenv["MODE_SNAKECLOCK"]
