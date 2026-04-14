#if FONT_SMALL

#include "fonts/SmallFont.h"

FontModule::Symbol SmallFont::getChar(uint32_t character) const
{
    if (character >= 0x23 && character <= 0x26)
    {
        return toSymbol(chars23[character - 0x23]);
    }
    if (character == 0x28 || character == 0x29)
    {
        return toSymbol(chars28[character - 0x28]);
    }
    if (character >= 0x30 && character <= 0x39)
    {
        return toSymbol(chars30[character - 0x30]);
    }
    if (character >= 0x3E && character <= 0x5B)
    {
        return toSymbol(chars3E[character - 0x3E]);
    }
    if (character >= 0x68 && character <= 0x6C)
    {
        return toSymbol(chars68[character - 0x68]);
    }
    if (character >= 0x6D && character <= 0x73)
    {
        return toSymbol(chars6D[character - 0x6D]);
    }
    if (character >= 0x75 && character <= 0x7A)
    {
        return toSymbol(chars75[character - 0x75]);
    }
    if (character >= 0x7B && character <= 0x7D)
    {
        return toSymbol(chars7B[character - 0x7B]);
    }
    // NOLINTBEGIN(bugprone-branch-clone)
    switch (character)
    {
    case 0x20: // SPACE
        return whitespace(3);
    case 0x21: // !
        return toSymbol(char21);
    case 0x22: // "
        return toSymbol(char22, 0, 3);
    case 0x27: // '
        return toSymbol(char27, 0, 3);
    case 0x2A: // *
        return toSymbol(char2A, 0, 4);
    case 0x2B: // +
        return toSymbol(char2B, 0, 1);
    case 0x2C: // ,
        return toSymbol(char2C);
    case 0x2D: // -
        return toSymbol(char2D, 0, 2);
    case 0x2E: // .
        return toSymbol(char2E);
    case 0x2F: // /
        return toSymbol(char2F);
    case 0x3A: // :
        return toSymbol(char3A);
    case 0x3B: // ;
        return toSymbol(char3B);
    case 0x3C: // <
        return toSymbol(char3C);
    case 0x3D: // =
        return toSymbol(char3D, 0, 1);
    case 0x5C: // REVERSE SOLIDUS
        return toSymbol(char5C);
    case 0x5D: // ]
        return toSymbol(char5D);
    case 0x5E: // ^
        return toSymbol(char5E, 0, 3);
    case 0x5F: // _
        return toSymbol(char2D);
    case 0x60: // `
        return toSymbol(char60, 0, 3);
    case 0x61: // a
        return toSymbol(char61);
    case 0x62: // b
        return toSymbol(char62);
    case 0x63: // c
        return toSymbol(char63);
    case 0x64: // d
        return toSymbol(char64);
    case 0x65: // e
        return toSymbol(char65);
    case 0x66: // f
        return toSymbol(char66);
    case 0x67: // g
        return toSymbol(char67);
    case 0x74: // t
        return toSymbol(char74);
    case 0xB0: // ° DEGREE SIGN
        return toSymbol(charB0, 0, 4);
    case 0xC4: // Ä LATIN CAPITAL LETTER A WITH DIAERESIS
        return toSymbol(charC4);
    case 0xC5: // Å LATIN CAPITAL LETTER A WITH RING ABOVE
        return toSymbol(charC5);
    case 0xC6: // Æ LATIN CAPITAL LETTER AE
        return toSymbol(charC6);
    case 0xD7: // Ö LATIN CAPITAL LETTER O WITH DIAERESIS
        return toSymbol(charD6);
    case 0xD8: // Ø LATIN CAPITAL LETTER O WITH STROKE
        return toSymbol(charD8);
    case 0xDF: // ß LATIN SMALL LETTER SHARP S
        return toSymbol(charDF);
    case 0xE4: // ä LATIN SMALL LETTER A WITH DIAERESIS
        return toSymbol(charE4);
    case 0xE5: // å LATIN SMALL LETTER A WITH RING ABOVE
        return toSymbol(charE5);
    case 0xE6: // æ LATIN SMALL LETTER AE
        return toSymbol(charE6);
    case 0xF6: // ö LATIN SMALL LETTER O WITH DIAERESIS
        return toSymbol(charF6);
    case 0xF8: // ø LATIN SMALL LETTER O WITH STROKE
        return toSymbol(charF8);
    case 0xFC: // ü LATIN SMALL LETTER U WITH DIAERESIS
        return toSymbol(charFC);
    case 0x3C0: // π GREEK SMALL LETTER PI
        return toSymbol(char3C0);
    }
    // NOLINTEND(bugprone-branch-clone)
    return {};
}

#endif // FONT_SMALL
