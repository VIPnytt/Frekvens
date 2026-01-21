#!/usr/bin/env python3

# Stream .csv graphic files provided by the Animation or Draw modes via E1.31.

import argparse
import csv
import httpx
import logging
import socket
import time


class E131Streamer:
    host: str
    mode: str = "Stream"
    rows: int
    sock: socket.socket = socket.socket(type=socket.SOCK_DGRAM)

    def __init__(
        self,
        host: str,
        rows: int = 16,
    ) -> None:
        self.host = host
        self.rows = rows
        logging.warning(
            "Deprecation: E131Streamer is deprecated. Use StreamCsv instead."
        )

    def __enter__(self):
        httpx.patch(f"http://{self.host}/restful/{self.mode}", json={"port": 5568})
        httpx.patch(f"http://{self.host}/restful/Modes", json={"mode": self.mode})
        self.sock.connect((self.host, 5568))
        return self

    def __exit__(self, exc_type, exc, tb):
        self.sock.close()

    def display(self, frame: list[list[int]]) -> None:
        packet = bytearray(126)
        packet.extend([pixel for row in frame for pixel in row])
        self.sock.send(packet)

    def parse(self, path: str) -> list[list[list[int]]]:
        with open(path, encoding="utf-8", newline="") as graphic:
            rows = [[int(pixel) for pixel in row] for row in csv.reader(graphic)]
            return [rows[i : i + self.rows] for i in range(0, len(rows), self.rows)]

    def stream(
        self, frames: list[list[list[int]]], interval: float | int = 0.5
    ) -> None:
        try:
            print("E1.31 stream started. Press Ctrl+C to terminate.")
            while True:
                for frame in frames:
                    self.display(frame)
                    time.sleep(interval)
        except KeyboardInterrupt:
            logging.info("Stream ended gracefully.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Stream .csv graphic files.")
    parser.add_argument("--host", help="Host", type=str)
    parser.add_argument("--interval", default=0.5, help="Frame interval", type=float)
    parser.add_argument("-i", "--input", help=".csv file path", type=str)
    parser.add_argument("--rows", default=16, help="Grid rows", type=int)
    args = parser.parse_args()
    if args.host is None:
        args.host = input("Host: ")
    if args.input is None:
        args.input = input(".csv file path: ")
    with E131Streamer(args.host, args.rows) as streamer:
        frames: list[list[list[int]]] = streamer.parse(args.input)
        if len(frames) > 1:
            streamer.stream(frames, args.interval)
        else:
            streamer.display(frames[0])
