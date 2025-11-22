#!/usr/bin/env python3

# Stream .csv files provided by the Animation or Draw modes via Art-Net.

import csv
import os
import socket
import time


class ArtNetStreamer:
    frames: list[bytearray] = []
    header: bytearray = bytearray(18)
    host: str
    interval: float
    port: int = 6454
    rows: int

    def __init__(
        self, host: str = "frekvens.local", interval: int = 500, rows: int = 16
    ) -> None:
        self.host = host
        self.interval = interval / 1_000
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
    host = input("Host: ")
    path = input("Path: ")
    if os.path.isfile(path):
        stream = ArtNetStreamer(host)
        stream.append(path)
        stream.begin()
    else:
        print(f"File not found")
        raise FileNotFoundError(path)
