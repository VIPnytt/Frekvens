#!/usr/bin/env python3

# Generate fonts.

import fontTools.ttLib
import matplotlib.font_manager
import os
import PIL.Image
import PIL.ImageDraw
import PIL.ImageFont
import unicodedata


def bitmap_compressor(bitmap: list[str]) -> tuple[list[str], int, int]:
    if not bitmap:
        return [], 0, 0
    height = len(bitmap)
    width = len(bitmap[0])
    top = 0
    while top < height and all(bit == "0" for bit in bitmap[top]):
        top += 1
    bottom = height - 1
    while bottom >= 0 and all(bit == "0" for bit in bitmap[bottom]):
        bottom -= 1
    left = 0
    while left < width and all(row[left] == "0" for row in bitmap):
        left += 1
    right = width - 1
    while right >= 0 and all(row[right] == "0" for row in bitmap):
        right -= 1
    if top > bottom or left > right:
        return [], left, height - 1 - bottom
    stripped = [row[left : right + 1] for row in bitmap[top : bottom + 1]]
    return stripped, left, height - 1 - bottom


def character_to_description(character: str) -> str | None:

    if character.isprintable() and not character.strip() and ord(character) != 0x5C:
        return character
    else:
        try:
            return unicodedata.name(character)
        except ValueError:
            return None


def characters_to_bitmaps(
    path: str, size: int, characters: list[str]
) -> dict[str, list[str]]:
    bitmaps = {}
    font = PIL.ImageFont.truetype(path, size)
    for character in characters:
        bbox = PIL.ImageDraw.Draw(PIL.Image.new("1", (1, 1), 0)).textbbox(
            (0, 0), character, font=font
        )
        img = PIL.Image.new("1", (int(bbox[2] - bbox[0]), int(bbox[3] - bbox[1])), 0)
        PIL.ImageDraw.Draw(img).text((-bbox[0], -bbox[1]), character, fill=1, font=font)
        bitmap = []
        for y in range(img.height):
            row = ""
            for x in range(img.width):
                row += "1" if img.getpixel((x, y)) else "0"
            bitmap.append(row)
        bitmaps[character] = bitmap
    return bitmaps


def font_to_characters(path: str) -> list[str]:
    font = fontTools.ttLib.TTFont(path)
    unicode: set[int] = set()
    for table in font["cmap"].tables:  # type: ignore
        if table.isUnicode():
            unicode.update(table.cmap.keys())
    characters = []
    for codepoint in sorted(unicode):
        try:
            characters.append(chr(codepoint))
        except ValueError:
            continue
    font.close()
    return characters


def font_to_name(path: str) -> tuple[str, str]:
    unique = os.path.splitext(os.path.basename(path))[0]
    full = unique
    font = fontTools.ttLib.TTFont(path)
    for record in font["name"].names:  # type: ignore
        if record.nameID == 4:
            full = record.toUnicode()
        elif record.nameID == 6:
            unique = record.toUnicode()
    font.close()
    return unique, full


def fonts_to_paths(query: str) -> list[str]:
    fonts = matplotlib.font_manager.findSystemFonts()
    fonts.sort()
    if query:
        return [font for font in fonts if query.lower() in font.lower()]
    return fonts


def main(path: str, size: int) -> None:
    bitmaps = characters_to_bitmaps(path, size, font_to_characters(path))
    _ascii = ""
    for cp in range(0x20, 0x80):
        if chr(cp) in bitmaps:
            bitmap, offsetX, offsetY = bitmap_compressor(bitmaps[chr(cp)])
            _ascii += "\n        {"
            _ascii += f"\n            // 0x{cp:X}, {character_to_description(chr(cp))}"
            if bitmap:
                _ascii += "\n            {"
                for row in bitmap:
                    _ascii += f"\n                0b{row},"
                _ascii += "\n            },"
            else:
                _ascii += "\n            {},"
            _ascii += f"\n            {offsetX},"
            _ascii += f"\n            {offsetY},"
            _ascii += "\n        },"
        else:
            _ascii += "\n        {},"
    _ascii += "\n    "
    _unicode = ""
    for character, _bitmap in bitmaps.items():
        bitmap, offsetX, offsetY = bitmap_compressor(_bitmap)
        if ord(character) >= 0x80 and ord(character) <= 0x10FFFF:
            _unicode += "\n        {"
            _unicode += f"\n            0x{ord(character):X},"
            comment = character_to_description(character)
            if comment:
                _unicode += f" // {comment}"
            _unicode += "\n            {"
            if bitmap:
                _unicode += "\n                {"
                for row in bitmap:
                    _unicode += f"\n                    0b{row},"
                _unicode += "\n                },"
            else:
                _unicode += "\n                {},"
            _unicode += f"\n                {offsetX},"
            _unicode += f"\n                {offsetY},"
            _unicode += "\n            },"
            _unicode += "\n        },"
    _unicode += "\n    "
    nameUnique, nameFull = font_to_name(path)
    with open(f"{nameUnique}Font.cpp", "w", encoding="utf-8") as cppFont:
        cppFont.write(
            f"""#include "fonts/{nameUnique}Font.h"

// This file was automatically generated
// {path}

{nameUnique}Font *Font{nameUnique} = nullptr;

{nameUnique}Font::{nameUnique}Font() : FontModule("{nameFull}")
{{
    Font{nameUnique} = this;
}}

FontModule::Symbol {nameUnique}Font::getChar(uint32_t character)
{{
    if (character >= 0x20 && character <= 0x7F && character < ascii.size() + 0x20)
    {{
        return ascii[character - 0x20];
    }}
    else if (character >= 0x80 && character <= 0x10FFFF)
    {{
        for (const SymbolExtended &extended : unicode)
        {{
            if (extended.hex == character)
            {{
                return extended.symbol;
            }}
        }}
    }}
    return {{}};
}}
"""
        )
    print(f"{nameUnique}Font.cpp")
    with open(f"{nameUnique}Font.h", "w", encoding="utf-8") as hFont:
        hFont.write(
            f"""#pragma once

#include "modules/FontModule.h"

// This file was automatically generated
// {path}

class {nameUnique}Font : public FontModule
{{
private:
    const std::vector<Symbol> ascii = {{{_ascii}}};

    const std::vector<SymbolExtended> unicode = {{{_unicode}}};

public:
    {nameUnique}Font();

    Symbol getChar(uint32_t character) override;
}};

extern {nameUnique}Font *Font{nameUnique};
"""
        )
    print(f"{nameUnique}Font.h")


if __name__ == "__main__":
    path = input("Font path: ")
    if os.path.isfile(path):
        main(path, int(input("Font size: ")))
    else:
        print(f"File not found")
        fonts = fonts_to_paths(path)
        if fonts:
            print("Alternative fonts to try:")
            for font in fonts:
                print(font)
        raise FileNotFoundError(path)
