#!/usr/bin/env python3

# Stream .csv graphic files provided by the Animation or Draw modes via Distributed Display Protocol, Art-Net or E1.31.

import argparse
import csv
import httpx
import socket
import time
import typing


class StreamCsv:
    header: bytearray
    host: str
    port: int
    protocol: str
    rows: int
    sock: socket.socket = socket.socket(type=socket.SOCK_DGRAM)

    def __init__(
        self,
        target: str = "frekvens.local",
        rows: int = 16,
    ) -> None:
        self.host, *port = target.rsplit(":", 1)
        self.port = int(port[0]) if port else 4048
        self.rows = rows
        if self.port == 4048:
            self.header = bytearray(10)
            self.protocol = "Distributed Display Protocol"
        elif self.port == 6454:
            self.header = bytearray(18)
            self.protocol = "Art-Net"
        elif self.port == 5568:
            self.header = bytearray(126)
            self.protocol = "E1.31"
        else:
            raise ValueError("Unsupported port number.")

    def parse(self, path: str) -> list[list[list[int]]]:
        with open(path, encoding="utf-8", newline="") as graphic:
            rows = [[int(pixel) for pixel in row] for row in csv.reader(graphic)]
            return [rows[i : i + self.rows] for i in range(0, len(rows), self.rows)]

    def send(
        self,
        data: list[list[int]] | list[list[list[int]]],
        interval: float | None = None,
    ) -> None:
        for frame in (
            [typing.cast(list[list[int]], data)]
            if isinstance(data[0][0], int)
            else typing.cast(list[list[list[int]]], data)
        ):
            packet = self.header.copy()
            packet.extend(pixel for row in frame for pixel in row)
            self.sock.sendto(packet, (self.host, self.port))
            if interval is not None:
                time.sleep(interval)

    def setMode(self) -> httpx.Response:
        return httpx.patch(
            f"http://{self.host}/restful/Modes", json={"mode": "Stream"}
        ).raise_for_status()

    def setPort(self) -> httpx.Response:
        return httpx.patch(
            f"http://{self.host}/restful/Stream", json={"port": self.port}
        ).raise_for_status()


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Stream .csv graphic files via Distributed Display Protocol, Art-Net or E1.31."
    )
    parser.add_argument("-i", "--input", help=".csv file path", required=True, type=str)
    parser.add_argument("--interval", help="Frame interval", type=float)
    parser.add_argument("--rows", help="Grid rows", type=int)
    parser.add_argument("target", help="hostname[:port]", nargs="?", type=str)
    args = parser.parse_args()
    kwargs = {
        key: value
        for key, value in {
            "rows": args.rows,
            "target": args.target,
        }.items()
        if value is not None
    }
    stream = StreamCsv(**kwargs)
    stream.setMode()
    stream.setPort()
    frames = stream.parse(args.input)
    if len(frames) > 1:
        try:
            print(f"{stream.protocol} stream started. Press Ctrl+C to terminate.")
            while True:
                stream.send(frames, args.interval)
        except KeyboardInterrupt:
            print("Stream ended gracefully.")
    else:
        stream.send(frames[0], args.interval)


if __name__ == "__main__":
    main()
