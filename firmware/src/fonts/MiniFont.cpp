#if FONT_MINI

#include "fonts/MiniFont.h"

FontModule::Symbol MiniFont::getChar(uint32_t character) const
{
    if (character >= 0x30 && character <= 0x39)
    {
        return toSymbol(chars30[character - 0x30]);
    }
    if (character >= 0x41 && character <= 0x5D)
    {
        return toSymbol(chars41[character - 0x41]);
    }
    if (character >= 0x65 && character <= 0x67)
    {
        return toSymbol(chars65[character - 0x65]);
    }
    if (character == 0x68 || character == 0x69)
    {
        return toSymbol(chars68[character - 0x68]);
    }
    if (character == 0x6B || character == 0x6C)
    {
        return toSymbol(chars6B[character - 0x6B]);
    }
    if (character == 0x6D || character == 0x6E)
    {
        return toSymbol(chars6D[character - 0x6D]);
    }
    if (character >= 0x6F && character <= 0x72)
    {
        return toSymbol(chars6F[character - 0x6F]);
    }
    if (character >= 0x75 && character <= 0x78)
    {
        return toSymbol(chars75[character - 0x75]);
    }
    // NOLINTBEGIN(bugprone-branch-clone)
    switch (character)
    {
    case 0x20: // SPACE
        return whitespace(3);
    case 0x2B: // +
        return toSymbol(char2B, 0, 1);
    case 0x2C: // ,
        return toSymbol(char2C);
    case 0x2D: // -
        return toSymbol(char2D, 0, 2);
    case 0x2E: // .
        return toSymbol(char2E);
    case 0x3A: // :
        return toSymbol(char3A, 0, 1);
    case 0x3D: // =
        return toSymbol(char3D, 0, 1);
    case 0x5F: // _
        return toSymbol(char2D);
    case 0x61: // a
        return toSymbol(char61);
    case 0x62: // b
        return toSymbol(char62);
    case 0x63: // c
        return toSymbol(char63);
    case 0x64: // d
        return toSymbol(char64);
    case 0x6A: // j
        return toSymbol(char6A);
    case 0x73: // s
        return toSymbol(char73);
    case 0x74: // t
        return toSymbol(char74);
    case 0x79: // y
        return toSymbol(char79);
    case 0x7A: // z
        return toSymbol(char7A);
    case 0x7C: // |
        return toSymbol(char7C);
    case 0xB0: // ° DEGREE SIGN
        return toSymbol(charB0, 0, 2);
    case 0x3C0: // π GREEK SMALL LETTER PI
        return toSymbol(char3C0);
    }
    // NOLINTEND(bugprone-branch-clone)
    return {};
}

#endif // FONT_MINI
