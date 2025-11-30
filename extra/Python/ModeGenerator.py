#!/usr/bin/env python3

# Generate mode source files from .csv files provided by the Animation or Draw modes.

import argparse
import csv
import os


class ModeGenerator:
    id: str
    name: str
    path: str
    rows: int

    def __init__(self, path: str, rows: int = 16) -> None:
        self.path = path
        self.rows = rows
        self.name = os.path.splitext(os.path.basename(path))[0]
        self.id = "".join(self.name.split()).capitalize()

    def create(self) -> None:
        with open(self.path, "r", encoding="utf-8") as animation:
            rows = csv.reader(animation)
            if sum(1 for _ in rows) <= self.rows:
                self._drawing_h()
                self._drawing_cpp()
            else:
                self._animation_h()
                self._animation_cpp()

    def _animation_h(self) -> None:
        frames = [
            "#pragma once",
            "",
            "#include <vector>",
            "",
            '#include "modules/ModeModule.h"',
            "",
            "//",
            "// @warning Automatically generated file",
            "//",
            "",
            f"class {self.id}Mode : public ModeModule",
            "{",
            "private:",
            "    unsigned long lastMillis = 0;",
            "",
            "    uint8_t index = 0;",
            "",
            "    std::vector<std::vector<uint16_t>> frames = {",
        ]
        with open(self.path) as animation:
            for i, row in enumerate(csv.reader(animation)):
                if i % self.rows == 0:
                    frames.append("        {")
                frames.append(
                    "            0b"
                    + "".join("1" if int(column) > 0 else "0" for column in row)
                    + ","
                )
                if i % self.rows == self.rows - 1:
                    frames.append("        },")
        frames.extend(
            [
                "    };",
                "",
                "public:",
                f'    {self.id}Mode() : ModeModule("{self.name}") {{}};',
                "",
                f"    void handle() override;",
                "};",
                "",
            ]
        )
        with open(f"{self.id}Mode.h", "w", encoding="utf-8") as hMode:
            hMode.write("\n".join(frames))
        print(f"{self.id}Mode.h")

    def _animation_cpp(self) -> None:
        with open(f"{self.id}Mode.cpp", "w", encoding="utf-8") as cppMode:
            cppMode.write(
                "\n".join(
                    [
                        '#include "config/constants.h"',
                        '#include "handlers/BitmapHandler.h"',
                        f'#include "modes/{self.id}Mode.h"',
                        '#include "services/DisplayService.h"',
                        "",
                        "//",
                        "// @warning Automatically generated file",
                        "//",
                        "",
                        f"void {self.id}Mode::handle()",
                        "{",
                        "    if (millis() - lastMillis >= 500)",
                        "    {",
                        "        lastMillis = millis();",
                        "        Display.clearFrame();",
                        "        BitmapHandler bitmap = BitmapHandler(frames[index]);",
                        "        bitmap.draw(GRID_COLUMNS - bitmap.getWidth(), 0);",
                        "        ++index;",
                        "        if (index >= frames.size())",
                        "        {",
                        "            index = 0;",
                        "        }",
                        "    }",
                        "}",
                        "",
                    ]
                )
            )
        print(f"{self.id}Mode.cpp")

    def _drawing_h(self) -> None:
        frame = [
            "#pragma once",
            "",
            "#include <vector>",
            "",
            '#include "modules/ModeModule.h"',
            "",
            "//",
            "// @warning Automatically generated file",
            "//",
            "",
            f"class {self.id}Mode : public ModeModule",
            "{",
            "private:",
            "    std::vector<uint16_t> frame = {",
        ]
        with open(self.path) as drawing:
            for i, row in enumerate(csv.reader(drawing)):
                if i > 0 and i % self.rows == 0:
                    frame.extend(
                        [
                            "    },",
                            "    {",
                        ]
                    )
                frame.append(
                    "        0b"
                    + "".join("1" if int(column) > 0 else "0" for column in row)
                    + ","
                )
        frame.extend(
            [
                "    };",
                "",
                "public:",
                f'    {self.id}Mode() : ModeModule("{self.name}") {{}};',
                "",
                f"    void begin() override;",
                "};",
                "",
            ]
        )
        with open(f"{self.id}Mode.h", "w", encoding="utf-8") as hMode:
            hMode.write("\n".join(frame))
        print(f"{self.id}Mode.h")

    def _drawing_cpp(self) -> None:
        with open(f"{self.id}Mode.cpp", "w", encoding="utf-8") as cppMode:
            cppMode.write(
                "\n".join(
                    [
                        '#include "config/constants.h"',
                        '#include "handlers/BitmapHandler.h"',
                        f'#include "modes/{self.id}Mode.h"',
                        '#include "services/DisplayService.h"',
                        "",
                        "//",
                        "// @warning Automatically generated file",
                        "//",
                        "",
                        f"void {self.id}Mode::begin()",
                        "{",
                        "    Display.clearFrame();",
                        "    BitmapHandler bitmap = BitmapHandler(frame);",
                        "    bitmap.draw(GRID_COLUMNS - bitmap.getWidth(), 0);",
                        "}",
                        "",
                    ]
                )
            )
        print(f"{self.id}Mode.cpp")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Generate mode source files from .csv files provided by the Animation or Draw modes."
    )
    parser.add_argument("-i", "--input", help=".csv file path", type=str)
    parser.add_argument("--rows", default=16, help="Grid rows", type=int)
    args = parser.parse_args()
    if args.input is None:
        args.input = input(".csv file path: ")
    ModeGenerator(args.input, args.rows).create()
