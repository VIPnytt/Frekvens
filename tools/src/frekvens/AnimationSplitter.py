#!/usr/bin/env python3

# Split .csv files from the Animation mode into individual frames or drawings.

import argparse
import os


class AnimationSplitter:
    rows: int

    def __init__(self, rows: int = 16) -> None:
        self.rows = rows

    def split(self, path: str = "Animation.csv") -> None:
        directory, filename = os.path.split(path)
        name, extension = os.path.splitext(filename)
        with open(path, "r", encoding="utf-8") as animation:
            rows = animation.readlines()
            for i, frame in enumerate(
                [rows[i : i + self.rows] for i in range(0, len(rows), self.rows)],
                start=1,
            ):
                _path = os.path.join(directory, f"{name}-frame-{i}{extension}")
                with open(_path, "w", encoding="utf-8", newline="") as drawing:
                    drawing.writelines(frame)
                print(f"#{i}: {_path}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Split .csv files from the Animation mode into individual frames or drawings."
    )
    parser.add_argument("-i", "--input", help=".csv file path", type=str)
    parser.add_argument("--rows", default=16, help="Grid rows", type=int)
    args = parser.parse_args()
    if args.input is None:
        args.input = input(".csv file path: ") or "Animation.csv"
    if os.path.isfile(args.input):
        AnimationSplitter(args.rows).split(args.input)
    else:
        raise FileNotFoundError(args.input)
