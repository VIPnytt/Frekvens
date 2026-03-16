#!/usr/bin/env python3

# Split .csv graphic files from the Animation mode into individual frames or drawings.

import argparse
import csv
import pathlib


class AnimationSplitter:
    rows: int

    def __init__(self, rows: int = 16) -> None:
        self.rows = rows

    def split(self, path: pathlib.Path | str) -> list[str]:
        _path = pathlib.Path(path)
        paths: list[str] = []
        for i, frame_rows in enumerate(self._parse(_path), 1):
            output = _path.with_name(f"{_path.stem}-{i}{_path.suffix}")
            with output.open("w", newline="") as f:
                csv.writer(f).writerows(frame_rows)
        return paths

    def _parse(self, path: pathlib.Path) -> list[list[list[int]]]:
        with path.open(newline="") as f:
            rows = [[int(cell) for cell in row] for row in csv.reader(f)]
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
