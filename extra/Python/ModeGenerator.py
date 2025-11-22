#!/usr/bin/env python3

# Generate mode source files from .csv files provided by the Animation or Draw modes.

import csv
import os


class ModeGenerator:
    name: str
    path: str
    rows: int

    def __init__(self, path: str = "Animation.csv", rows: int = 16) -> None:
        self.name = os.path.splitext(os.path.basename(self.path))[0]
        self.path = path
        self.rows = rows

    def source(self) -> None:
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
            "// This file was automatically generated",
            "",
            f"class {self.name}Mode : public ModeModule",
            "{",
            "private:",
            "    unsigned long lastMillis = 0;",
            "",
            "    uint8_t index = 0;",
            "    std::vector<std::vector<uint16_t>> frames = {",
        ]
        with open(self.path) as animation:
            for i, row in enumerate(csv.reader(animation)):
                if i > 0 and i % self.rows == 0:
                    frames.extend(
                        [
                            "    },",
                            "    {",
                        ]
                    )
                frames.append(
                    "            0b"
                    + "".join("1" if int(column) > 0 else "0" for column in row)
                    + ","
                )
        frames.extend(
            [
                "        };",
                "",
                "public:",
                f'    {self.name}Mode() : ModeModule("{self.name}") {{}};',
                "",
                f"    void handle() override;",
                "};",
                "",
            ]
        )
        with open(f"{self.name}Mode.h", "w", encoding="utf-8") as hMode:
            hMode.write("\n".join(frames))
        print(f"{self.name}Mode.h")

    def _animation_cpp(self) -> None:
        with open(f"{self.name}Mode.cpp", "w", encoding="utf-8") as cppMode:
            cppMode.write(
                "".join(
                    [
                        '#include "config/constants.h"',
                        '#include "handlers/BitmapHandler.h"',
                        f'#include "modes/{self.name}Mode.h"',
                        '#include "services/DisplayService.h"',
                        "",
                        "// This file was automatically generated",
                        "",
                        f"void {self.name}Mode::handle()",
                        "{",
                        "    if (millis() - lastMillis >= 500)",
                        "    {",
                        "        lastMillis = millis();",
                        "        Display.clear();",
                        "        BitmapHandler handler = BitmapHandler(frames[index]);",
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
        print(f"{self.name}Mode.cpp")

    def _drawing_h(self) -> None:
        frame = [
            "#pragma once",
            "",
            "#include <vector>",
            "",
            '#include "modules/ModeModule.h"',
            "",
            "// This file was automatically generated",
            "",
            f"class {self.name}Mode : public ModeModule",
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
                f'    {self.name}Mode() : ModeModule("{self.name}") {{}};',
                "",
                f"    void wake() override;",
                "};",
                "",
            ]
        )
        with open(f"{self.name}Mode.h", "w", encoding="utf-8") as hMode:
            hMode.write("\n".join(frame))
        print(f"{self.name}Mode.h")

    def _drawing_cpp(self) -> None:
        with open(f"{self.name}Mode.cpp", "w", encoding="utf-8") as cppMode:
            cppMode.write(
                "".join(
                    [
                        '#include "config/constants.h"',
                        '#include "handlers/BitmapHandler.h"',
                        f'#include "modes/{self.name}Mode.h"',
                        '#include "services/DisplayService.h"',
                        "",
                        "// This file was automatically generated",
                        "",
                        f"void {self.name}Mode::wake()",
                        "{",
                        "    if (millis() - lastMillis >= 500)",
                        "    {",
                        "        Display.clear();",
                        "        BitmapHandler handler = BitmapHandler(frame);",
                        "        handler.draw(GRID_COLUMNS - handler.getWidth(), 0)",
                        "    }",
                        "}",
                        "",
                    ]
                )
            )
        print(f"{self.name}Mode.cpp")


if __name__ == "__main__":
    path = input("Path: ")
    if os.path.isfile(path):
        ModeGenerator(path).source()
    else:
        print(f"File not found")
        raise FileNotFoundError(path)
