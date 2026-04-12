#!/usr/bin/env python3

# Generate mode source files from .csv files provided by the Animation or Draw modes.

import argparse
import csv
import pathlib


class ModeGenerator:
    id: str
    name: str
    path: pathlib.Path
    rows: int

    def __init__(self, path: pathlib.Path | str, rows: int = 16) -> None:
        self.path = pathlib.Path(path)
        self.rows = rows
        self.name = self.path.stem
        self.id = "".join(self.name.split()).capitalize()

    def create(self) -> list[str]:
        with self.path.open(newline="") as f:
            count = sum(1 for _ in csv.reader(f))
        if count <= self.rows:
            return [
                self._drawing_h(count),
                self._drawing_cpp(),
            ]
        else:
            return [
                self._animation_h(count),
                self._animation_cpp(),
            ]

    def _animation_h(self, count: int) -> str:
        frames = [
            "#pragma once",
            "",
            '#include "modules/ModeModule.h"',
            "",
            "#include <array>",
            "",
            "//",
            "// @warning Automatically generated file",
            "//",
            "",
            f"class {self.id}Mode final : public ModeModule",
            "{",
            "private:",
            "    unsigned long lastMillis = 0;",
            "",
            "    uint8_t index = 0;",
            "",
            f"    static constexpr std::array<std::array<uint16_t, {self.rows}>, {count // self.rows}> frames{{{{",
        ]
        with self.path.open(newline="") as animation:
            for i, row in enumerate(csv.reader(animation)):
                if i % self.rows == 0:
                    frames.append("        {")
                frames.append(f"            0b{self._bits(row)},")
                if i % self.rows == self.rows - 1:
                    frames.append("        },")
        frames.extend(
            [
                "    }};",
                "",
                "public:",
                f'    explicit {self.id}Mode() : ModeModule("{self.name}") {{}};',
                "",
                "    void handle() override;",
                "};",
                "",
            ]
        )
        with open(f"{self.id}Mode.h", "w", encoding="utf-8") as f:
            f.write("\n".join(frames))
        return f"{self.id}Mode.h"

    def _animation_cpp(self) -> str:
        with open(f"{self.id}Mode.cpp", "w", encoding="utf-8") as f:
            f.write(
                "\n".join(
                    [
                        f'#include "modes/{self.id}Mode.h"',
                        "",
                        '#include "config/constants.h" // NOLINT(misc-include-cleaner)',
                        '#include "handlers/BitmapHandler.h"',
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
                        "        const BitmapHandler bitmap(frames[index]);",
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
        return f"{self.id}Mode.cpp"

    def _drawing_h(self, count: int) -> str:
        frame = [
            "#pragma once",
            "",
            '#include "modules/ModeModule.h"',
            "",
            "#include <array>",
            "",
            "//",
            "// @warning Automatically generated file",
            "//",
            "",
            f"class {self.id}Mode final : public ModeModule",
            "{",
            "private:",
            f"    static constexpr std::array<uint16_t, {count}> frame{{",
        ]
        with self.path.open(newline="") as drawing:
            for row in csv.reader(drawing):
                frame.append(f"        0b{self._bits(row)},")
        frame.extend(
            [
                "    };",
                "",
                "public:",
                f'    explicit {self.id}Mode() : ModeModule("{self.name}") {{}};',
                "",
                "    void begin() override;",
                "};",
                "",
            ]
        )
        with open(f"{self.id}Mode.h", "w", encoding="utf-8") as f:
            f.write("\n".join(frame))
        return f"{self.id}Mode.h"

    def _drawing_cpp(self) -> str:
        with open(f"{self.id}Mode.cpp", "w", encoding="utf-8") as f:
            f.write(
                "\n".join(
                    [
                        f'#include "modes/{self.id}Mode.h"',
                        "",
                        '#include "config/constants.h" // NOLINT(misc-include-cleaner)',
                        '#include "handlers/BitmapHandler.h"',
                        '#include "services/DisplayService.h"',
                        "",
                        "//",
                        "// @warning Automatically generated file",
                        "//",
                        "",
                        f"void {self.id}Mode::begin()",
                        "{",
                        "    Display.clearFrame();",
                        "    const BitmapHandler bitmap(frame);",
                        "    bitmap.draw(GRID_COLUMNS - bitmap.getWidth(), 0);",
                        "}",
                        "",
                    ]
                )
            )
        return f"{self.id}Mode.cpp"

    @staticmethod
    def _bits(row: list[str]) -> str:
        return "".join("1" if int(column) > 0 else "0" for column in row)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Generate mode source files from .csv files provided by the Animation or Draw modes."
    )
    parser.add_argument("-i", "--input", help=".csv file path", required=True, type=str)
    parser.add_argument("--rows", help="Grid rows", type=int)
    args = parser.parse_args()
    kwargs = {
        key: value
        for key, value in {
            "path": args.input,
            "rows": args.rows,
        }.items()
        if value is not None
    }
    for path in ModeGenerator(**kwargs).create():
        print(path)


if __name__ == "__main__":
    main()
