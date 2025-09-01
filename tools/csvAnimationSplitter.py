#!/usr/bin/env python3

# Split .csv files from the "Animation" mode into multiple frames or drawings.

import os

from src.config.constants import ROWS

if not ROWS:
    ROWS = 16  # px


def main(path: str) -> None:
    path, filename = os.path.split(path)
    name, extension = os.path.splitext(filename)
    with open(path) as csvArt:
        rows = csvArt.readlines()
        chunks = [rows[i : i + ROWS] for i in range(0, len(rows), ROWS)]
        for frame, chunk in enumerate(chunks):
            with open(f"{name}-{frame+1}{extension}", "w", encoding="utf-8") as file:
                file.writelines(chunk)
                print(f"{name}-{frame+1}{extension}")


if __name__ == "__main__":
    path = input(".csv file path: ")
    if os.path.isfile(path):
        main(path)
    else:
        print(f"File not found")
        raise FileNotFoundError(path)
