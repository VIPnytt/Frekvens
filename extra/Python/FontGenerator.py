#!/usr/bin/env python3

# Generate font source files from .ttf or .otf files.

import argparse
import fontTools.ttLib
import logging
import matplotlib.font_manager
import pathlib
import PIL.Image
import PIL.ImageDraw
import PIL.ImageFont
import unicodedata


class FontGenerator:
    characters: dict[int, tuple[int, int, str | None]]
    path: pathlib.Path
    size: int

    def __init__(self, path: pathlib.Path | str, size: int = 8) -> None:
        self.path = pathlib.Path(path)
        self.size = size

    def _character_to_description(self, character: str) -> str | None:
        if character.isprintable() and character.strip() and ord(character) != 0x5C:
            if ord(character) >= 0x80:
                try:
                    return f"{character} {unicodedata.name(character)}"
                except ValueError as e:
                    logging.debug("Character name for %s not found: %s", character, e)
            return character
        else:
            try:
                return unicodedata.name(character)
            except ValueError as e:
                logging.debug("Character name for %s not found: %s", character, e)
                return None

    def _characters_to_bitmaps(self, characters: list[str], index: int = 0) -> dict[str, list[str]]:
        bitmaps = {}
        font = PIL.ImageFont.truetype(self.path, self.size, index)
        probe = PIL.Image.new("1", (1, 1), 0)
        draw = PIL.ImageDraw.Draw(probe)
        for character in characters:
            bbox = draw.textbbox((0, 0), character, font=font)
            width = max(0, int(bbox[2] - bbox[0]))
            height = max(0, int(bbox[3] - bbox[1]))
            if width == 0 or height == 0:
                bitmaps[character] = []
                continue
            img = PIL.Image.new("1", (width, height), 0)
            PIL.ImageDraw.Draw(img).text((-bbox[0], -bbox[1]), character, fill=1, font=font)
            bitmaps[character] = [
                "".join("1" if img.getpixel((x, y)) else "0" for x in range(img.width)) for y in range(img.height)
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
        codepoints: set[int] = set()
        with fontTools.ttLib.TTFont(self.path) as font:
            for table in font["cmap"].tables:
                if table.isUnicode():
                    codepoints.update(table.cmap.keys())
        characters = []
        for codepoint in sorted(codepoints):
            try:
                characters.append(chr(codepoint))
            except ValueError:
                continue
        return characters

    def source(self) -> list[str]:
        self.characters = {}
        unique = self.path.stem
        name = unique
        with fontTools.ttLib.TTFont(self.path) as font:
            for record in font["name"].names:
                if record.nameID == 4:
                    name = record.toUnicode()
                elif record.nameID == 6:
                    unique = record.toUnicode()
        return [
            self._source_h(unique, name),
            self._source_cpp(unique),
        ]

    def _source_h(self, unique: str, name: str) -> str:
        h = [
            "#pragma once",
            "",
            '#include "modules/FontModule.h"',
            "",
            "#include <array>",
            "",
            "//",
            "// @warning Automatically generated file",
            "//",
            "",
            f"class {unique}Font final : public FontModule",
            "{",
            "private:",
        ]
        bitmaps = self._characters_to_bitmaps(self._font_to_characters())
        for character in bitmaps:
            bitmap, offsetX, offsetY = self._crop(bitmaps[character])
            if bitmap:
                cp = ord(character)
                comment = self._character_to_description(character)
                self.characters[cp] = (offsetX, offsetY, comment)
                if comment:
                    h.append(f"    // 0x{cp:X}, {comment}")
                else:
                    h.append(f"    // 0x{cp:X}")
                h.append(
                    f"    static constexpr std::array<uint{max(8, 1 << (len(bitmap[0]) - 1).bit_length())}_t, {len(bitmap)}> _{cp:X} = {{"
                )
                for row in bitmap:
                    h.append(f"        0b{row},")
                h.append("    };")
                h.append("")
        h.extend(
            [
                "",
                "public:",
                f'    static constexpr std::string_view name = "{name}";',
                "",
                f"    explicit {unique}Font() : FontModule(name) {{}};",
                "",
                "    [[nodiscard]] Symbol getChar(uint32_t character) const override;",
                "};",
                "",
            ]
        )
        pathlib.Path(f"{unique}Font.h").write_text("\n".join(h), encoding="utf-8")
        return f"{unique}Font.h"

    def _source_cpp(self, unique: str) -> str:
        cpp = [
            f'#include "fonts/{unique}Font.h"',
            "",
            "//",
            "// @warning Automatically generated file",
            "//",
            "",
            f"FontModule::Symbol {unique}Font::getChar(uint32_t character) const",
            "{",
            "    switch (character)",
            "    {",
        ]
        count = 0
        prefix = ""
        for cp, (offsetX, offsetY, comment) in self.characters.items():
            if count == (1 << 7):
                prefix = "    // "
            count += 1
            if comment:
                cpp.append(f"    {prefix}case 0x{cp:X}: // {comment}")
            else:
                cpp.append(f"    {prefix}case 0x{cp:X}:")
            cpp.append(f"    {prefix}    return {{_{cp:X}, {offsetX}, {offsetY}}};")
        cpp.extend(
            [
                "    }",
                "    return {};",
                "}",
                "",
            ]
        )
        pathlib.Path(f"{unique}Font.cpp").write_text("\n".join(cpp), encoding="utf-8")
        return f"{unique}Font.cpp"

    @staticmethod
    def find(query: str) -> list[str]:
        fonts = matplotlib.font_manager.findSystemFonts()
        if query:
            return [font for font in fonts if query.lower() in pathlib.Path(font).name.lower()]
        return fonts


def main() -> None:
    parser = argparse.ArgumentParser(description="Generate font source files from .ttf or .otf files.")
    parser.add_argument("-i", "--input", help="Font path", required=True, type=str)
    parser.add_argument("--size", help="Font size", type=int)
    args = parser.parse_args()
    input_path = pathlib.Path(args.input)
    kwargs = {
        key: value
        for key, value in {
            "path": input_path,
            "size": args.size,
        }.items()
        if value is not None
    }
    paths: list[str] = []
    if input_path.is_file():
        paths = FontGenerator(**kwargs).source()
    else:
        fonts = FontGenerator.find(args.input)
        for font in fonts:
            path = pathlib.Path(font)
            if path.stem.lower() == args.input.lower():
                paths = FontGenerator(path).source() if args.size is None else FontGenerator(path, args.size).source()
                break
    if not paths:
        raise FileNotFoundError(f"Font not found: '{args.input}'")
    for _path in paths:
        print(_path)


if __name__ == "__main__":
    main()
