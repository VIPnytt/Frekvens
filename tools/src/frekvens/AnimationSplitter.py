#!/usr/bin/env python3

# Split .csv graphic files from the Animation mode into individual frames or drawings.

import argparse
import csv
import os


class AnimationSplitter:
    rows: int

    def __init__(self, rows: int = 16) -> None:
        self.rows = rows

    def split(self, path: str) -> list[str]:
        directory, filename = os.path.split(path)
        name, extension = os.path.splitext(filename)
        paths: list[str] = []
        for i, rows in enumerate(self._parse(path), 1):
            paths.append(os.path.join(directory, f"{name}-{i}{extension}"))
            with open(paths[-1], "w", encoding="utf-8", newline="") as frame:
                writer = csv.writer(frame)
                writer.writerows(rows)
        return paths

    def _parse(self, path: str) -> list[list[list[int]]]:
        with open(path, encoding="utf-8", newline="") as graphic:
            rows = [[int(cell) for cell in row] for row in csv.reader(graphic)]
            return [rows[i : i + self.rows] for i in range(0, len(rows), self.rows)]


def main() -> None:
    parser = argparse.ArgumentParser(description="Split .csv graphic files into individual frames.")
    parser.add_argument("-i", "--input", help=".csv file path", required=True, type=str)
    parser.add_argument("--rows", help="Grid rows", type=int)
    args = parser.parse_args()
    splitter = AnimationSplitter(args.rows) if args.rows else AnimationSplitter()
    for path in splitter.split(args.input):
        print(path)


if __name__ == "__main__":
    main()
