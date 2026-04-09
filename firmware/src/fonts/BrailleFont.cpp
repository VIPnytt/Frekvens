#if FONT_BRAILLE

#include "fonts/BrailleFont.h"

#include <vector>

FontModule::Symbol BrailleFont::toChar(uint8_t bits) const
{
    const uint8_t row1 = bits & 0x03;
    const uint8_t row2 = (bits >> 2) & 0x03;
    const uint8_t row3 = (bits >> 4) & 0x03;
    const int8_t size = row3 ? 3 : row2 ? 2 : row1 ? 1 : 0;
    std::vector<uint8_t> bitmap(size);
    if (size)
    {
        bitmap[0] = row1;
    }
    if (size >= 2)
    {
        bitmap[1] = row2;
    }
    if (size == 3)
    {
        bitmap[2] = row3;
    }
    return {bitmap, ((row1 | row2 | row3) & 0x02) ? 0 : 1, 3 - size};
}

FontModule::Symbol BrailleFont::getChar(uint32_t character) const
{
    if (character >= 0x31 && character <= 0x39)
    {
        return toChar(_41[character - 0x31]);
    }
    if (character >= 0x41 && character <= 0x5A)
    {
        return toChar(_41[character - 0x41]);
    }
    if (character >= 0x61 && character <= 0x7A)
    {
        return toChar(_41[character - 0x61]);
    }
    switch (character)
    {
    case 0x20: // SPACE
        return {{}, 2, 0};
    case 0x21: // !
        return toChar(0b1110);
    case 0x27: // '
        return toChar(0b10);
    case 0x28: // (
    case 0x29: // )
    case 0x5B: // [
    case 0x5D: // ]
    case 0x7B: // {
    case 0x7D: // }
        return toChar(0b1111);
    case 0x2C: // ,
        return toChar(0b1000);
    case 0x2D: // -
        return toChar(0b11);
    case 0x2E: // .
        return toChar(0b1101);
    case 0x2F: // /
        return toChar(0b10010);
    case 0x30: // 0
        return toChar(_41[9]);
    case 0x3A: // :
        return toChar(0b1100);
    case 0x3B: // ;
        return toChar(0b1010);
    case 0x3F: // ?
        return toChar(0b1011);
    }
    return {};
}

#endif // FONT_BRAILLE
