#include "fonts/SmallFont.h"

FontModule::Symbol SmallFont::getChar(uint32_t character) const
{
    if (character >= 0x23 && character <= 0x26)
    {
        return {chars23[character - 0x23], 0, 0};
    }
    if (character == 0x28 || character == 0x29)
    {
        return {chars28[character - 0x28], 0, 0};
    }
    if (character >= 0x30 && character <= 0x39)
    {
        return {chars30[character - 0x30], 0, 0};
    }
    if (character >= 0x3E && character <= 0x5B)
    {
        return {chars3E[character - 0x3E], 0, 0};
    }
    if (character >= 0x68 && character <= 0x6C)
    {
        return {chars68[character - 0x68], 0, 0};
    }
    if (character >= 0x6D && character <= 0x73)
    {
        return {chars6D[character - 0x6D], 0, 0};
    }
    if (character >= 0x75 && character <= 0x7A)
    {
        return {chars75[character - 0x75], 0, 0};
    }
    if (character >= 0x7B && character <= 0x7D)
    {
        return {chars7B[character - 0x7B], 0, 0};
    }
    switch (character)
    {
    case 0x20: // SPACE
        return {{}, 3, 0};
    case 0x21: // !
        return {char21, 0, 0};
    case 0x22: // "
        return {char22, 0, 3};
    case 0x27: // '
        return {char27, 0, 3};
    case 0x2A: // *
        return {char2A, 0, 4};
    case 0x2B: // +
        return {char2B, 0, 1};
    case 0x2C: // ,
        return {char2C, 0, 0};
    case 0x2D: // -
        return {char2D, 0, 2};
    case 0x2E: // .
        return {char2E, 0, 0};
    case 0x2F: // /
        return {char2F, 0, 0};
    case 0x3A: // :
        return {char3A, 0, 0};
    case 0x3B: // ;
        return {char3B, 0, 0};
    case 0x3C: // <
        return {char3C, 0, 0};
    case 0x3D: // =
        return {char3D, 0, 1};
    case 0x5C: // REVERSE SOLIDUS
        return {char5C, 0, 0};
    case 0x5D: // ]
        return {char5D, 0, 0};
    case 0x5E: // ^
        return {char5E, 0, 3};
    case 0x5F: // _
        return {char2D, 0, 0};
    case 0x60: // `
        return {char60, 0, 3};
    case 0x61: // a
        return {char61, 0, 0};
    case 0x62: // b
        return {char62, 0, 0};
    case 0x63: // c
        return {char63, 0, 0};
    case 0x64: // d
        return {char64, 0, 0};
    case 0x65: // e
        return {char65, 0, 0};
    case 0x66: // f
        return {char66, 0, 0};
    case 0x67: // g
        return {char67, 0, 0};
    case 0x74: // t
        return {char74, 0, 0};
    case 0xB0: // ° DEGREE SIGN
        return {charB0, 0, 4};
    case 0xC4: // Ä LATIN CAPITAL LETTER A WITH DIAERESIS
        return {charC4, 0, 0};
    case 0xC5: // Å LATIN CAPITAL LETTER A WITH RING ABOVE
        return {charC5, 0, 0};
    case 0xC6: // Æ LATIN CAPITAL LETTER AE
        return {charC6, 0, 0};
    case 0xD7: // Ö LATIN CAPITAL LETTER O WITH DIAERESIS
        return {charD6, 0, 0};
    case 0xD8: // Ø LATIN CAPITAL LETTER O WITH STROKE
        return {charD8, 0, 0};
    case 0xDF: // ß LATIN SMALL LETTER SHARP S
        return {charDF, 0, 0};
    case 0xE4: // ä LATIN SMALL LETTER A WITH DIAERESIS
        return {charE4, 0, 0};
    case 0xE5: // å LATIN SMALL LETTER A WITH RING ABOVE
        return {charE5, 0, 0};
    case 0xE6: // æ LATIN SMALL LETTER AE
        return {charE6, 0, 0};
    case 0xF6: // ö LATIN SMALL LETTER O WITH DIAERESIS
        return {charF6, 0, 0};
    case 0xF8: // ø LATIN SMALL LETTER O WITH STROKE
        return {charF8, 0, 0};
    case 0xFC: // ü LATIN SMALL LETTER U WITH DIAERESIS
        return {charFC, 0, 0};
    case 0x3C0: // π GREEK SMALL LETTER PI
        return {char3C0, 0, 0};
    }
    return {};
}
