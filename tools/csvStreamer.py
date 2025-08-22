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
from src.modes import ArtNet, DistributedDisplayProtocol, E131

if not ROWS:
    ROWS = 16


def main():
    try:
        filepath = input(".csv file path: ")
        if not os.path.isfile(filepath):
            raise RuntimeError("Read error")
        frames = []
        with open(filepath, newline="", encoding="utf-8") as csvfile:
            i = 0
            reader = csv.reader(csvfile)
            for row in reader:
                if not i % ROWS:
                    frames.append([])
                for value in row:
                    frames[-1].append(int(value.strip()))
                i += 1

        duration = 500
        if len(frames) > 1:
            _duration = input(f"Frame duration (default is {duration} ms): ")
            if len(_duration) and int(_duration) > 0:
                duration = int(_duration)
        begin(frames, duration)
    except KeyboardInterrupt:
        pass


def begin(frames, duration):
    if MODE_DISTRIBUTEDDISPLAYPROTOCOL:
        with DistributedDisplayProtocol.DistributedDisplayProtocol(HOST) as mode:
            handle(mode, frames, duration)
    elif MODE_ARTNET:
        with ArtNet.ArtNet(HOST) as mode:
            handle(mode, frames, duration)
    elif MODE_E131:
        with E131.E131(HOST) as mode:
            handle(mode, frames, duration)
    else:
        raise RuntimeError("No streaming extensions available.")


def handle(mode, frames, duration):
    count = len(frames)
    if count > 1:
        print(f"{mode.NAME} stream started. Press Ctrl+C to terminate gracefully.")
    frame = 0
    while True:
        mode.send(frames[frame % count])
        if count < 2:
            break
        frame += 1
        time.sleep(duration / 1000)


if __name__ == "__main__":
    main()
