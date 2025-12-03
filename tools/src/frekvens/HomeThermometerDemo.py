#!/usr/bin/env python3

# Demo script for setting the Home thermometer temperatures.

import argparse
import httpx


class HomeThermometerDemo:
    host: str
    mode: str = "Home thermometer"

    def __init__(self, host: str) -> None:
        self.host = host

    def set(
        self,
        indoor: float | int | None = None,
        outdoor: float | int | None = None,
        activate: bool = False,
    ) -> None:
        httpx.patch(
            f"http://{self.host}/restful/{self.mode}",
            json={
                where: temperature
                for where, temperature in {
                    "indoor": indoor,
                    "outdoor": outdoor,
                }.items()
                if temperature is not None
            },
        ).raise_for_status()
        if activate:
            httpx.patch(f"http://{self.host}/restful/Modes", json={"mode": self.mode})


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Set the Home thermometer temperatures."
    )
    parser.add_argument("--host", help="Host", type=str)
    parser.add_argument("--indoor", help="Indoor temperature", type=float)
    parser.add_argument("--outdoor", help="Outdoor temperature", type=float)
    args = parser.parse_args()
    if args.host is None:
        args.host = input("Host: ")
    if args.indoor is None:
        indoor = input("Indoor temperature: ") or None
        args.indoor = None if indoor is None or not len(indoor) else float(indoor)
    if args.outdoor is None:
        outdoor = input("Outdoor temperature: ") or None
        args.outdoor = None if outdoor is None or not len(outdoor) else float(outdoor)
    HomeThermometerDemo(args.host).set(args.indoor, args.outdoor, True)
