import datetime
import importlib.util
import locale
import logging
import pathlib
import typing
import tzlocal
import zoneinfo

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens


class Time:
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        self.project = project

    def configure(self) -> None:
        iana, posix = self._get_zone()
        self.project.dotenv["TIME_ZONE"] = iana
        self.project.dotenv["TIME_ZONE_POSIX"] = posix
        if "CLOCK_FORMAT" in self.project.dotenv:
            if (format := self.project.dotenv.get("CLOCK_FORMAT")) in {"12", "24"}:
                self.project.dotenv[f"CLOCK_{format}H"] = "true"
            else:
                logging.warning(
                    "CLOCK_FORMAT %r is unsupported. Valid values are '12' and '24'.",
                    self.project.dotenv["CLOCK_FORMAT"],
                )
        else:
            previous = locale.setlocale(locale.LC_TIME)
            try:
                locale.setlocale(locale.LC_TIME, "")
                clock = datetime.datetime.now().replace(hour=23).strftime("%X")
                if clock.startswith("23"):
                    self.project.dotenv["CLOCK_24H"] = "true"
                elif clock.startswith("11"):
                    self.project.dotenv["CLOCK_12H"] = "true"
            finally:
                locale.setlocale(locale.LC_TIME, previous)

    def _get_zone(self) -> tuple[str, str]:
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
        paths = [pathlib.Path(path) for path in zoneinfo.TZPATH]
        spec = importlib.util.find_spec("tzdata")
        if spec and spec.submodule_search_locations:
            paths.extend(pathlib.Path(path) / "zoneinfo" for path in spec.submodule_search_locations)
        for path in dict.fromkeys(paths):
            zone_file = path / iana
            if zone_file.is_file():
                zone = zone_file.read_bytes()
                if zone.startswith(b"TZif") and zone.endswith(b"\n"):
                    if posix := zone.rsplit(b"\n", 2)[-2].decode("ascii", errors="strict"):
                        return posix
        logging.warning(f"Unknown timezone: {iana}")
        return None
