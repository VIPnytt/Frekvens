#!/usr/bin/env python3

# Stream .csv files provided by the "Animation" or "Draw" modes using "Distributed Display Protocol", "Art-Net" or "E1.31".

import csv
import os
import time

from src.config.constants import (
    HOST,
    ROWS,
    MODE_ARTNET,
    MODE_DISTRIBUTEDDISPLAYPROTOCOL,
    MODE_E131,
)
from src.modes.Animation import Animation
from src.modes.ArtNet import ArtNet
from src.modes.DistributedDisplayProtocol import DistributedDisplayProtocol
from src.modes.E131 import E131

if not ROWS:
    ROWS = 16  # px


class csvStreamer:
    def __init__(self, duration: int = Animation.DURATION) -> None:
        self.duration = duration
        self.frames: list[list[int]] = []

    def read(self, filepath: str) -> None:
        with open(filepath, newline="", encoding="utf-8") as csvfile:
            i = 0
            reader = csv.reader(csvfile)
            for row in reader:
                if not i % ROWS:
                    self.frames.append([])
                for value in row:
                    self.frames[-1].append(int(value.strip()))
                i += 1

    def begin(self) -> None:
        if MODE_DISTRIBUTEDDISPLAYPROTOCOL:
            with DistributedDisplayProtocol(HOST) as mode:
                self._handle(mode)
        elif MODE_ARTNET:
            with ArtNet(HOST) as mode:
                self._handle(mode)
        elif MODE_E131:
            with E131(HOST) as mode:
                self._handle(mode)
        else:
            raise NotImplementedError("No streaming extensions available.")

    def _handle(self, mode) -> None:
        count = len(self.frames)
        if count > 1:
            print(f"{mode.NAME} stream started. Press Ctrl+C to terminate gracefully.")
        frame = 0
        try:
            while True:
                mode.send(self.frames[frame % count])
                if count < 2:
                    break
                frame += 1
                time.sleep(self.duration / 1000)
        except KeyboardInterrupt:
            pass


if __name__ == "__main__":
    path = input(".csv file path: ")
    if os.path.isfile(path):
        duration = int(input(f"Frame duration (default {Animation.DURATION} ms): "))
        if not duration:
            duration = Animation.DURATION
        stream = csvStreamer(duration)
        stream.read(path)
        stream.begin()
    else:
        print(f"File not found")
        raise FileNotFoundError(path)
