#!/usr/bin/env python3

# Generate modes from .csv files provided by the "Animation" or "Draw" modes.

import csv
import os
import re
import sys

from src.config.constants import ROWS

if not ROWS:
    ROWS = 16


def check(_file) -> bool:
    if (
        os.path.isdir("firmware/include/modes/")
        and os.path.isdir("firmware/src/modes/")
        and not os.path.isfile(f"firmware/include/modes/{_file}Mode.h")
        and not os.path.isfile(f"firmware/src/modes/{_file}Mode.cpp")
    ):
        return True
    else:
        return False


name = input("Mode name: ").strip()
file = re.sub(r"[^a-zA-Z]", "", name).title()

if len(file) < 2:
    sys.exit("Name too short")

filepath = input(".csv file path: ")
if not os.path.isfile(filepath):
    sys.exit("Read error")

# .csv downloaded via webapp
with open(filepath) as csvArt:
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

# firmware/include/modes/GeneratedMode.h
with open(f"firmware/include/modes/{file}Mode.h", "w") as hMode:
    hMode.write(
        f"""#pragma once

#include <vector>

#include "modules/ModeModule.h"

// This file was automatically generated
class {file}Mode : public ModeModule
{{
private:
    {variable}

public:
    {file}Mode() : ModeModule("{name}") {{}};

    void {method}() override;
}};
"""
    )
    print(f"firmware/include/modes/{file}Mode.h")

# firmware/src/modes/GeneratedMode.cpp
with open(f"firmware/src/modes/{file}Mode.cpp", "w") as cppMode:
    if method == "handle":
        cppMode.write(
            f"""#include "config/constants.h"
#include "handlers/BitmapHandler.h"
#include "modes/{file}Mode.h"
#include "services/DisplayService.h"

// This file was automatically generated
void {file}Mode::{method}()
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
#include "modes/{file}Mode.h"
#include "services/DisplayService.h"

// This file was automatically generated
void {file}Mode::{method}()
{{
    Display.clear();
    BitmapHandler handler = BitmapHandler(frame);
    handler.draw(COLUMNS - handler.getWidth(), 0);
}}
"""
        )
    print(f"firmware/src/modes/{file}Mode.cpp")

print("The mode has been generated successfully!")
print("Modify the generated files as needed.")
print("When done, add the mode to firmware/include/ModesService.h")
