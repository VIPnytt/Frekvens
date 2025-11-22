#!/usr/bin/env python3

# Split .csv files from the Animation mode into individual frames or drawings.

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
                _path = os.path.join(directory, f"{name}-frame{i}{extension}")
                with open(_path, "w", encoding="utf-8", newline="") as drawing:
                    drawing.writelines(frame)
                print(f"#{i}: {_path}")


if __name__ == "__main__":
    path = input("Path: ")
    if os.path.isfile(path):
        splitter = AnimationSplitter()
        splitter.split(path)
    else:
        print(f"File not found")
        raise FileNotFoundError(path)
