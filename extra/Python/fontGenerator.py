#!/usr/bin/env python3

# Generate font source files from .ttf or .otf files.

import fontTools.ttLib
import matplotlib.font_manager
import os
import PIL.Image
import PIL.ImageDraw
import PIL.ImageFont
import unicodedata


class FontGenerator:
    path: str
    size: int

    def __init__(self, path: str = "Font.ttf", size: int = 8) -> None:
        self.path = path
        self.size = size

    def _character_to_description(self, character: str) -> str | None:
        if character.isprintable() and not character.strip() and ord(character) != 0x5C:
            return character
        else:
            try:
                return unicodedata.name(character)
            except ValueError:
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
            bitmap = []
            for y in range(img.height):
                row = ""
                for x in range(img.width):
                    row += "1" if img.getpixel((x, y)) else "0"
                bitmap.append(row)
            bitmaps[character] = bitmap
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

    def source(self) -> None:
        unique = os.path.splitext(os.path.basename(self.path))[0]
        name = unique
        font = fontTools.ttLib.TTFont(self.path)
        for record in font["name"].names:  # type: ignore
            if record.nameID == 4:
                name = record.toUnicode()
            elif record.nameID == 6:
                unique = record.toUnicode()
        font.close()
        self._source_h(unique)
        self._source_cpp(unique, f"{name} {self.size}")

    def _source_h(self, unique: str) -> None:
        bitmaps = self._characters_to_bitmaps(self._font_to_characters())
        font = [
            "#pragma once",
            "",
            '#include "modules/FontModule.h"',
            "",
            "// This file was automatically generated",
            "",
            f"class {unique}Font : public FontModule",
            "{",
            "private:",
            "    const std::vector<Symbol> ascii = {",
        ]
        for cp in range(0x20, 0x80):
            if chr(cp) in bitmaps:
                bitmap, offsetX, offsetY = self._crop(bitmaps[chr(cp)])
                font.extend(
                    [
                        "        {",
                        f"            // 0x{cp:X}, {self._character_to_description(chr(cp))}",
                    ]
                )
                if bitmap:
                    font.append("            {")
                    for row in bitmap:
                        font.append(f"                0b{row},")
                    font.append("            },")
                else:
                    font.append("            {},")
                font.extend(
                    [
                        f"            {offsetX},",
                        f"            {offsetY},",
                        "        },",
                    ]
                )
            else:
                font.append("        {},")
        font.extend(
            [
                "    };",
                "    const std::vector<SymbolExtended> unicode = {",
            ]
        )
        for character, _bitmap in bitmaps.items():
            bitmap, offsetX, offsetY = self._crop(_bitmap)
            if ord(character) >= 0x80 and ord(character) <= 0x10FFFF:
                comment = self._character_to_description(character)
                font.append("        {")
                if comment:
                    font.append(f"            0x{ord(character):X}, // {comment}")
                else:
                    font.append(f"            0x{ord(character):X},")
                font.append("            {")
                if bitmap:
                    font.append("                {")
                    for row in bitmap:
                        font.append(f"                    0b{row},")
                    font.append("                },")
                else:
                    font.append("                {},")
                font.extend(
                    [
                        f"                {offsetX},",
                        f"                {offsetY},",
                        "            },",
                        "        },",
                    ]
                )
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
            h.write("".join(font))
        print(f"{unique}Font.h")

    def _source_cpp(self, unique: str, name: str) -> None:
        with open(f"{unique}Font.cpp", "w", encoding="utf-8") as cpp:
            cpp.write(
                "\n".join(
                    [
                        f'#include "fonts/{unique}Font.h"',
                        "",
                        "// This file was automatically generated",
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
    def search(query: str) -> list[str]:
        fonts = matplotlib.font_manager.findSystemFonts()
        fonts.sort()
        if query:
            return [font for font in fonts if query.lower() in font.lower()]
        return fonts


if __name__ == "__main__":
    path = input("Path: ")
    size = int(input("Size: "))
    if os.path.isfile(path) and size > 1:
        FontGenerator(path, size).source()
    else:
        print(f"File not found")
        fonts = FontGenerator.search(path)
        if fonts:
            print("Alternative fonts to try:")
            for font in fonts:
                print(font)
        raise FileNotFoundError(path)
