#!/usr/bin/env python3

# Demo script for setting the Home thermometer temperatures.

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
    host = input("Host: ")
    indoor = float(input("Indoor: "))
    outdoor = float(input("Outdoor: "))
    thermometer = HomeThermometerDemo(host)
    thermometer.indoor(indoor)
    thermometer.outdoor(outdoor)
    thermometer.push()
