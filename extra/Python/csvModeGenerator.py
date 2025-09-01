#!/usr/bin/env python3

# Generate modes from .csv files provided by the "Animation" or "Draw" modes.

import csv
import os

from tools.src.config.constants import ROWS

if not ROWS:
    ROWS = 16  # px


def main(path: str) -> None:
    name = os.path.splitext(os.path.basename(path))[0]

    with open(path) as csvArt:
        rows = csv.reader(csvArt)
        method = "wake" if sum(1 for row in rows) <= ROWS else "handle"
        csvArt.seek(0)
        rowNo = 1
        rows = csv.reader(csvArt)
        if method == "handle":
            variable = f"""unsigned long lastMillis = 0;

    uint8_t index = 0;
    std::vector<std::vector<uint16_t>> frames = {{
        {{"""
        else:
            variable = f"std::vector<uint16_t> frame = {{"
        for row in rows:
            if rowNo > ROWS:
                variable += f"\n    }},\n    {{"
                rowNo = 1
            variable += "\n        0b" if method == "wake" else "\n            0b"
            for column in row:
                variable += "1" if int(column) > 0 else "0"
            variable += ","
            rowNo += 1
        if method == "wake":
            variable += "\n    };"
        else:
            variable += "\n        }};"

    with open(f"{name}Mode.h", "w", encoding="utf-8") as hMode:
        hMode.write(
            f"""#pragma once

#include <vector>

#include "modules/ModeModule.h"

// This file was automatically generated
// {path}

class {name}Mode : public ModeModule
{{
private:
    {variable}

public:
    {name}Mode() : ModeModule("{name}") {{}};

    void {method}() override;
}};
"""
        )
        print(f"{name}Mode.h")

    with open(f"{name}Mode.cpp", "w", encoding="utf-8") as cppMode:
        if method == "handle":
            cppMode.write(
                f"""#include "config/constants.h"
#include "handlers/BitmapHandler.h"
#include "modes/{name}Mode.h"
#include "services/DisplayService.h"

// This file was automatically generated
void {name}Mode::{method}()
{{
    if (millis() - lastMillis > 500) // 0.5 seconds between each frame
    {{
        lastMillis = millis();
        Display.clear();
        BitmapHandler handler = BitmapHandler(frames[index]);
        handler.draw(COLUMNS - handler.getWidth(), 0);
        ++index; // Next
        if (index >= frames.size())
        {{
            index = 0; // Restart
        }}
    }}
}}
"""
            )
        elif method == "wake":
            cppMode.write(
                f"""#include "config/constants.h"
#include "handlers/BitmapHandler.h"
#include "modes/{name}Mode.h"
#include "services/DisplayService.h"

// This file was automatically generated
// {path}

void {name}Mode::{method}()
{{
    Display.clear();
    BitmapHandler handler = BitmapHandler(frame);
    handler.draw(COLUMNS - handler.getWidth(), 0);
}}
"""
            )
        print(f"{name}Mode.cpp")


if __name__ == "__main__":
    path = input(".csv file path: ")
    if os.path.isfile(path):
        main(path)
    else:
        print(f"File not found")
        raise FileNotFoundError(path)
