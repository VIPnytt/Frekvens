# Split .csv files from the "Animation" mode into multiple frames or drawings.

import os
import src.config.constants
import sys


if src.config.constants.ROWS is None:
    src.config.constants.ROWS = 16

filepath = input(".csv file path: ")

if not os.path.isfile(filepath):
    sys.exit("Read error")

path, filename = os.path.split(filepath)
name, extension = os.path.splitext(filename)

# .csv downloaded via webapp
with open(filepath) as csvArt:
    rows = csvArt.readlines()
    chunks = [
        rows[i : i + src.config.constants.ROWS]
        for i in range(0, len(rows), src.config.constants.ROWS)
    ]
    for frame, chunk in enumerate(chunks):
        with open(os.path.join(path, f"{name}-{frame+1}{extension}"), "w") as file:
            file.writelines(chunk)
            print(os.path.join(path, f"{name}-{frame+1}{extension}"))
