#!/usr/bin/env python3

# Set the Home thermometer temperatures.

import argparse
import httpx
import urllib.parse


class HomeThermometerUpdater:
    host: str
    mode: str = "Home thermometer"

    def __init__(self, host: str = "frekvens.local") -> None:
        self.host = host

    def set(
        self, indoor: float | None = None, outdoor: float | None = None
    ) -> httpx.Response:
        return httpx.patch(
            f"http://{self.host}/restful/{urllib.parse.quote(self.mode)}",
            json={
                where: temperature
                for where, temperature in {
                    "indoor": indoor,
                    "outdoor": outdoor,
                }.items()
                if temperature is not None
            },
        ).raise_for_status()

    def setMode(self) -> httpx.Response:
        return httpx.patch(
            f"http://{self.host}/restful/Modes", json={"mode": self.mode}
        ).raise_for_status()


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Set the Home thermometer temperatures."
    )
    parser.add_argument("host", help="Hostname", nargs="?", type=str)
    parser.add_argument("--indoor", help="Indoor temperature", type=float)
    parser.add_argument("--outdoor", help="Outdoor temperature", type=float)
    parser.add_argument("--switch", action="store_true", help="Switch mode?")
    args = parser.parse_args()
    thermometer = (
        HomeThermometerUpdater(args.host) if args.host else HomeThermometerUpdater()
    )
    if args.indoor is not None or args.outdoor is not None:
        thermometer.set(args.indoor, args.outdoor)
    if args.switch:
        thermometer.setMode()


if __name__ == "__main__":
    main()
