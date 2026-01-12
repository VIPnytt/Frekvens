#!/usr/bin/env python3

# Generate font source files from .ttf or .otf files.

import argparse
import fontTools.ttLib
import logging
import matplotlib.font_manager
import os
import PIL.Image
import PIL.ImageDraw
import PIL.ImageFont
import unicodedata


class FontGenerator:
    count: int
    path: str
    size: int

    def __init__(self, path: str, size: int = 8) -> None:
        self.count = 0
        self.path = path
        self.size = size

    def _character_to_description(self, character: str) -> str | None:
        if character.isprintable() and character.strip() and ord(character) != 0x5C:
            if ord(character) >= 0x80:
                try:
                    return f"{character} {unicodedata.name(character)}"
                except ValueError as e:
                    logging.debug("Character name not found: %s", e)
            return character
        else:
            try:
                return unicodedata.name(character)
            except ValueError as e:
                logging.debug("Character name not found: %s", e)
                return None

    def _characters_to_bitmaps(
        self, characters: list[str], index: int = 0
    ) -> dict[str, list[str]]:
        bitmaps = {}
        font = PIL.ImageFont.truetype(self.path, self.size, index)
        for character in characters:
            bbox = PIL.ImageDraw.Draw(PIL.Image.new("1", (1, 1), 0)).textbbox(
                (0, 0), character, font=font
            )
            img = PIL.Image.new(
                "1", (int(bbox[2] - bbox[0]), int(bbox[3] - bbox[1])), 0
            )
            PIL.ImageDraw.Draw(img).text(
                (-bbox[0], -bbox[1]), character, fill=1, font=font
            )
            bitmaps[character] = [
                "".join("1" if img.getpixel((x, y)) else "0" for x in range(img.width))
                for y in range(img.height)
            ]
        return bitmaps

    def _crop(self, bitmap: list[str]) -> tuple[list[str], int, int]:
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

    def _font_to_characters(self) -> list[str]:
        font = fontTools.ttLib.TTFont(self.path)
        unicode: set[int] = set()
        for table in font["cmap"].tables:
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

    def source(self) -> None:
        unique = os.path.splitext(os.path.basename(self.path))[0]
        name = unique
        font = fontTools.ttLib.TTFont(self.path)
        for record in font["name"].names:
            if record.nameID == 4:
                name = record.toUnicode()
            elif record.nameID == 6:
                unique = record.toUnicode()
        font.close()
        self._source_h(unique)
        self._source_cpp(unique, name)

    def _source_h(self, unique: str) -> None:
        bitmaps = self._characters_to_bitmaps(self._font_to_characters())
        font = [
            "#pragma once",
            "",
            '#include "modules/FontModule.h"',
            "",
            "//",
            "// @warning Automatically generated file",
            "//",
            "",
            f"class {unique}Font : public FontModule",
            "{",
            "private:",
            "    const std::vector<Symbol> ascii = {",
        ]
        for cp in range(0x20, 0x80):
            character = chr(cp)
            if character in bitmaps:
                bitmap, offsetX, offsetY = self._crop(bitmaps[character])
                font.extend(
                    [
                        "        {",
                        f"            // 0x{cp:X}, {self._character_to_description(character)}",
                    ]
                )
                if bitmap:
                    font.append("            {")
                    if len(bitmap[0]) > 8:
                        font.append("                /*")
                    for row in bitmap:
                        font.append(f"                0b{row},")
                    if len(bitmap[0]) > 8:
                        font.append("                */")
                    font.append("            },")
                else:
                    font.append("            {},")
                    if cp == 0x20:
                        offsetX = round(self.size / 2)
                font.extend(
                    [
                        f"            {offsetX},",
                        f"            {offsetY},",
                        "        },",
                    ]
                )
                self.count += 1
            else:
                font.append("        {},")
        font.extend(
            [
                "    };",
                "",
                "    const std::vector<SymbolExtended> unicode = {",
            ]
        )
        for character, _bitmap in bitmaps.items():
            bitmap, offsetX, offsetY = self._crop(_bitmap)
            cp = ord(character)
            if cp >= 0x80 and cp <= 0x10FFFF:
                comment = self._character_to_description(character)
                if self.count >= 2**10 or (bitmap and len(bitmap[0]) > 8):
                    font.append("        /*")
                font.append("        {")
                if comment:
                    font.append(f"            0x{cp:X}, // {comment}")
                else:
                    font.append(f"            0x{cp:X},")
                font.append("            {")
                if bitmap:
                    font.append("                {")
                    for row in bitmap:
                        font.append(f"                    0b{row},")
                    font.append("                },")
                else:
                    font.append("                {},")
                if cp == 0xA0:
                    offsetX = round(self.size / 2)
                font.extend(
                    [
                        f"                {offsetX},",
                        f"                {offsetY},",
                        "            },",
                        "        },",
                    ]
                )
                if self.count >= 2**10 or (bitmap and len(bitmap[0]) > 8):
                    font.append("        */")
                else:
                    self.count += 1
        font.append("    };")
        with open(f"{unique}Font.h", "w", encoding="utf-8") as h:
            font.extend(
                [
                    "",
                    "public:",
                    f"    {unique}Font();",
                    "",
                    "    Symbol getChar(uint32_t character) const override;",
                    "};",
                    "",
                    f"extern {unique}Font *Font{unique};",
                    "",
                ]
            )
            h.write("\n".join(font))
        print(f"{unique}Font.h")

    def _source_cpp(self, unique: str, name: str) -> None:
        with open(f"{unique}Font.cpp", "w", encoding="utf-8") as cpp:
            cpp.write(
                "\n".join(
                    [
                        f'#include "fonts/{unique}Font.h"',
                        "",
                        "//",
                        "// @warning Automatically generated file",
                        "//",
                        "",
                        f"{unique}Font *Font{unique} = nullptr;",
                        "",
                        f'{unique}Font::{unique}Font() : FontModule("{name}")',
                        "{",
                        f"    Font{unique} = this;",
                        "}",
                        "",
                        f"FontModule::Symbol {unique}Font::getChar(uint32_t character) const",
                        "{",
                        "    if (character >= 0x20 && character <= 0x7F && character < ascii.size() + 0x20)",
                        "    {",
                        "        return ascii[character - 0x20];",
                        "    }",
                        "    else if (character >= 0x80 && character <= 0x10FFFF)",
                        "    {",
                        "        for (const SymbolExtended &extended : unicode)",
                        "        {",
                        "            if (extended.hex == character)",
                        "            {",
                        "                return extended.symbol;",
                        "            }",
                        "        }",
                        "    }",
                        "    return {};",
                        "}",
                        "",
                    ]
                )
            )
        print(f"{unique}Font.cpp")

    @staticmethod
    def find(query: str) -> list[str]:
        fonts = matplotlib.font_manager.findSystemFonts()
        if query:
            return [
                font
                for font in fonts
                if query.lower() in os.path.basename(font).lower()
            ]
        return fonts


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Generate font source files from .ttf or .otf files."
    )
    parser.add_argument("-i", "--input", help="Font path", type=str)
    parser.add_argument("--size", help="Font size", type=int)
    args = parser.parse_args()
    if args.input is None:
        args.input = input("Font file path: ")
    if args.size is None:
        args.size = int(input("Font size: ") or 8)
    try:
        FontGenerator(args.input, args.size).source()
    except FileNotFoundError:
        match = False
        fonts = FontGenerator.find(args.input)
        for font in fonts:
            if os.path.splitext(os.path.basename(font))[0] == args.input:
                FontGenerator(font, args.size).source()
                match = True
                break
        if not match:
            raise
