import os
import zoneinfo
import importlib.util


class Network:
    def tz_lookup(self, tz_name: str) -> str | None:
        search_paths = []
        if zoneinfo.TZPATH:
            search_paths.extend(zoneinfo.TZPATH)
        spec = importlib.util.find_spec("tzdata")
        if spec and spec.submodule_search_locations:
            search_paths.append(
                os.path.join(spec.submodule_search_locations[0], "zoneinfo")
            )
        for tz_path in search_paths:
            zone_file = os.path.join(tz_path, tz_name)
            if os.path.exists(zone_file):
                with open(zone_file, "rb") as f:
                    data = f.read()
                if b"\x00" in data:
                    posix = (
                        data.split(b"\x00")[-1].decode("ascii", errors="ignore").strip()
                    )
                    if posix:
                        return posix
        return None
