import datetime
import importlib.util
import locale
import logging
import pathlib
import typing
import zoneinfo

import tzlocal

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens
    from .Types import COMMAND_LINE_TARGETS
else:
    from SCons.Script import COMMAND_LINE_TARGETS


class Time:
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        """Initialize the time component with its associated project.
        
        Parameters:
            project (Frekvens): Project whose time configuration is managed.
        """
        self.project = project

    def initialize(self) -> None:
        """Disable project time configuration for filesystem build and upload targets."""
        if COMMAND_LINE_TARGETS in [
            ["buildfs"],
            ["uploadfs"],
            ["uploadfsota"],
        ]:
            self.project.time = None

    def configure(self) -> None:
        """
        Configure the project's timezone and clock display settings.
        
        The timezone is stored in IANA and POSIX formats. The clock display preference is
        set from ``CLOCK_FORMAT`` when provided, or inferred from the system locale.
        Unsupported clock format values are ignored.
        
        """
        iana, posix = self._get_zone()
        self.project.dotenv["TIME_ZONE"] = iana
        self.project.dotenv["TIME_ZONE_POSIX"] = posix
        format = self.project.dotenv.get("CLOCK_FORMAT")
        if format is None:
            previous = locale.setlocale(locale.LC_TIME)
            try:
                locale.setlocale(locale.LC_TIME, "")
                clock = {
                    "11": "CLOCK_12H",
                    "23": "CLOCK_24H",
                }.get(datetime.datetime.now(datetime.timezone.utc).replace(hour=23).strftime("%X")[:2])
                if clock is not None:
                    self.project.dotenv[clock] = "true"
            finally:
                locale.setlocale(locale.LC_TIME, previous)
        else:
            clock = {
                "12": "CLOCK_12H",
                "24": "CLOCK_24H",
            }.get(format)
            if clock is None:
                logging.warning("CLOCK_FORMAT %r is unsupported. Valid values are '12' and '24'.", format)
            else:
                self.project.dotenv[clock] = "true"

    def _get_zone(self) -> tuple[str, str]:
        """
        Resolve the configured or system timezone for the project.
        
        Returns:
        	tuple[str, str]: The IANA timezone name and its corresponding POSIX timezone string.
        """
        if "TIME_ZONE" in self.project.dotenv:
            posix = self._lookup_zone(self.project.dotenv["TIME_ZONE"])
            if posix:
                return self.project.dotenv["TIME_ZONE"], posix
        if iana := tzlocal.get_localzone_name():
            posix = self._lookup_zone(iana)
            if posix:
                return iana, posix
        return "Etc/UTC", "UTC0"

    @staticmethod
    def _lookup_zone(iana: str) -> str | None:
        """Resolve an IANA timezone identifier to its embedded POSIX timezone string.
        
        Parameters:
        	iana (str): IANA timezone identifier to resolve.
        
        Returns:
        	str: The embedded POSIX timezone string.
        	None: If the timezone cannot be found or contains invalid data.
        """
        paths = [pathlib.Path(path) for path in zoneinfo.TZPATH]
        spec = importlib.util.find_spec("tzdata")
        if spec and spec.submodule_search_locations:
            paths.extend(pathlib.Path(path) / "zoneinfo" for path in spec.submodule_search_locations)
        for path in dict.fromkeys(paths):
            zone_file = path / iana
            if zone_file.is_file():
                zone = zone_file.read_bytes()
                if zone.startswith(b"TZif") and zone.endswith(b"\n"):
                    posix = zone.rsplit(b"\n", 2)[-2].decode("ascii", errors="strict")
                    if posix:
                        return posix
        logging.warning("Unknown timezone: %r", iana)
        return None
