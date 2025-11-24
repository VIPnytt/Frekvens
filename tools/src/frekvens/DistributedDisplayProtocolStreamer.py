#!/usr/bin/env python3

# Stream .csv files provided by the Animation or Draw modes via Distributed Display Protocol.

import argparse
import csv
import os
import socket
import time


class DistributedDisplayProtocolStreamer:
    frames: list[bytearray] = []
    header: bytearray = bytearray(10)
    host: str
    interval: float | int
    port: int = 4048
    rows: int

    def __init__(
        self, host: str = "frekvens.local", interval: float | int = 0.5, rows: int = 16
    ) -> None:
        self.host = host
        self.interval = interval
        self.rows = rows

    def append(self, path: str = "Animation.csv") -> None:
        with open(path, newline="", encoding="utf-8") as animation:
            frame = self.header
            i = 0
            for row in csv.reader(animation):
                for column in row:
                    frame.append(int(column.strip()))
                i += 1
                if i % self.rows == 0:
                    self.frames.append(frame)
                    frame = self.header

    def begin(self) -> None:
        frames = len(self.frames)
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        if frames > 1:
            print(f"Stream started. Press Ctrl+C to terminate.")
        i = 0
        try:
            while True:
                sock.sendto(self.frames[i % frames], (self.host, self.port))
                i += 1
                if frames < 2:
                    break
                time.sleep(self.interval)
        except KeyboardInterrupt:
            sock.close()
            print(f"Stream ended gracefully.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Stream .csv files provided by the Animation or Draw modes via Distributed Display Protocol."
    )
    parser.add_argument("--host", help="Host or IP address", type=str)
    parser.add_argument(
        "--interval", default=0.5, help="Frame interval (seconds)", type=float
    )
    parser.add_argument("-i", "--input", help=".csv file path", type=str)
    parser.add_argument("--rows", default=16, help="Grid rows", type=int)
    args = parser.parse_args()
    if args.host is None:
        args.host = input("Host or IP address: ") or "frekvens.local"
    if args.input is None:
        args.input = input(".csv file path: ") or "Animation.csv"
    if os.path.isfile(args.input):
        stream = DistributedDisplayProtocolStreamer(args.host, args.interval, args.rows)
        stream.append(args.input)
        stream.begin()
    else:
        raise FileNotFoundError(args.input)
