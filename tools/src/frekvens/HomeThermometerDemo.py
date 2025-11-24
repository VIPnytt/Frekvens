#!/usr/bin/env python3

# Demo script for setting the Home thermometer temperatures.

import argparse
import httpx


class HomeThermometerDemo:
    host: str
    temperatures: dict[str, float | int]

    def __init__(self, host: str = "frekvens.local") -> None:
        self.host = host
        self.temperatures = {}

    def indoor(self, temperature: float | int) -> None:
        self.temperatures["indoor"] = temperature

    def outdoor(self, temperature: float | int) -> None:
        self.temperatures["outdoor"] = temperature

    def push(self) -> bool:
        return httpx.patch(
            f"http://{self.host}/RESTful/Home thermometer", json=self.temperatures
        ).is_success


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Demo script for setting the Home thermometer temperatures."
    )
    parser.add_argument("--host", help="Host or IP address", type=str)
    parser.add_argument("--indoor", help="Indoor temperature", type=float)
    parser.add_argument("--outdoor", help="Outdoor temperature", type=float)
    args = parser.parse_args()
    if args.host is None:
        args.host = input("Host or IP address: ") or "frekvens.local"
    if args.indoor is None:
        args.indoor = input("Indoor temperature: ") or None
    if args.outdoor is None:
        args.outdoor = input("Outdoor temperature: ") or None
    thermometer = HomeThermometerDemo(args.host)
    if args.indoor is not None:
        thermometer.indoor(float(args.indoor))
    if args.outdoor is not None:
        thermometer.outdoor(float(args.outdoor))
    if args.indoor is not None or args.outdoor is not None:
        thermometer.push()
