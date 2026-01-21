import importlib.util
import os
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
        posix = self._get_posix()
        if posix:
            self.project.dotenv["TIME_ZONE"] = posix

    def _get_posix(self) -> str | None:
        if "TIME_ZONE" in self.project.dotenv:
            return self._lookup(self.project.dotenv["TIME_ZONE"])
        system = tzlocal.get_localzone_name()
        if system:
            return self._lookup(system)
        return None

    @staticmethod
    def _lookup(iana: str) -> str | None:
        paths = []
        if zoneinfo.TZPATH:
            paths.extend(zoneinfo.TZPATH)
        spec = importlib.util.find_spec("tzdata")
        if spec and spec.submodule_search_locations:
            paths.append(os.path.join(spec.submodule_search_locations[0], "zoneinfo"))
        for path in paths:
            zone_file = os.path.join(path, iana)
            if os.path.exists(zone_file):
                with open(zone_file, "rb") as f:
                    lines = [
                        line.strip() for line in f.read().decode("ascii", errors="ignore").splitlines() if line.strip()
                    ]
                    if len(lines) >= 2:
                        return lines[-1]
        warnings.warn(f"Unknown timezone: {iana}", UserWarning)
        return None
