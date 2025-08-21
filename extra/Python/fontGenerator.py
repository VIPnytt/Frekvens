#!/usr/bin/env python3

# Template script for generating character bitmaps.

import matplotlib.font_manager
import PIL.Image, PIL.ImageFont, PIL.ImageDraw

ASCII = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"


def fonts_list():
    fonts = matplotlib.font_manager.findSystemFonts(fontpaths=None, fontext="ttf")
    fonts.sort()
    for font in fonts:
        print(font)


def text_to_bitmap(font_path, size, text):
    for character in text:
        font = PIL.ImageFont.truetype(font_path, size)
        bbox = PIL.ImageDraw.Draw(PIL.Image.new("1", (1, 1), 0)).textbbox(
            (0, 0), character, font=font
        )
        img = PIL.Image.new("1", (int(bbox[2] - bbox[0]), int(bbox[3] - bbox[1])), 0)
        PIL.ImageDraw.Draw(img).text((-bbox[0], -bbox[1]), character, fill=1, font=font)
        bitmap = []
        for y in range(img.height):
            row = []
            for x in range(img.width):
                pixel = img.getpixel((x, y))
                row.append(1 if pixel else 0)
            bitmap.append(row)
        cp = ord(character)
        print(
            f"{{ 0x{cp:X}, // {character}"
            if cp > 0x7F
            else f"{{ // 0x{cp:X}, {character}"
        )
        print("{{" if cp > 0x7F else "{")
        for row in bitmap:
            print(
                "0b"
                + "".join("1" if px else "0" for px in row)
                + ", // "
                + "".join("@" if px else "." for px in row)
            )
        print("},0,0,},}," if cp > 0x7F else "},0,0,},")


if __name__ == "__main__":
    # fonts_list()
    text_to_bitmap("arial.ttf", 8, ASCII)
