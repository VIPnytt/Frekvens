import logging
import typing

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens


class Deprecated:
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        self.project = project

    def initialize(self) -> None:
        self._largetickingclock()
        self._smalltickingclock()

    def _largetickingclock(self) -> None:
        if "MODE_LARGETICKINGCLOCK" in self.project.dotenv:
            logging.warning(
                'Deprecation: MODE_LARGETICKINGCLOCK "Large ticking clock" is deprecated. Use MODE_LARGECLOCK "Large clock" instead.'
            )
            if "MODE_LARGECLOCK" not in self.project.dotenv:
                self.project.dotenv["MODE_LARGECLOCK"] = self.project.dotenv[
                    "MODE_LARGETICKINGCLOCK"
                ]
            del self.project.dotenv["MODE_LARGETICKINGCLOCK"]

    def _smalltickingclock(self) -> None:
        if "MODE_SMALLTICKINGCLOCK" in self.project.dotenv:
            logging.warning(
                'Deprecation: MODE_SMALLTICKINGCLOCK "Small ticking clock" is deprecated. Use MODE_SMALLCLOCK "Small clock" instead.'
            )
            if "MODE_SMALLCLOCK" not in self.project.dotenv:
                self.project.dotenv["MODE_SMALLCLOCK"] = self.project.dotenv[
                    "MODE_SMALLTICKINGCLOCK"
                ]
            del self.project.dotenv["MODE_SMALLTICKINGCLOCK"]
