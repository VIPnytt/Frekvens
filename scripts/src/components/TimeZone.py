import importlib.util
import pathlib
import typing
import tzlocal
import warnings
import zoneinfo

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens


class TimeZone:
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        self.project = project

    def configure(self) -> None:
        if posix := self._get_posix():
            self.project.dotenv["TIME_ZONE"] = posix

    def _get_posix(self) -> str | None:
        if "TIME_ZONE" in self.project.dotenv:
            return self._lookup(self.project.dotenv["TIME_ZONE"])
        if system := tzlocal.get_localzone_name():
            return self._lookup(system)
        return None

    @staticmethod
    def _lookup(iana: str) -> str | None:
        paths = [pathlib.Path(path) for path in zoneinfo.TZPATH]
        spec = importlib.util.find_spec("tzdata")
        if spec and spec.submodule_search_locations:
            paths.extend(pathlib.Path(path) / "zoneinfo" for path in spec.submodule_search_locations)
        for path in dict.fromkeys(paths):
            zone_file = path / iana
            if zone_file.is_file():
                lines = [
                    line.strip()
                    for line in zone_file.read_text(encoding="ascii", errors="ignore").splitlines()
                    if line.strip()
                ]
                if len(lines) >= 2:
                    return lines[-1]
        warnings.warn(f"Unknown timezone: {iana}", UserWarning)
        return None
