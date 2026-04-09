#include "fonts/SmallFont.h"

FontModule::Symbol SmallFont::getChar(uint32_t character) const
{
    if (character >= 0x23 && character <= 0x26)
    {
        return {_23[character - 0x23], 0, 0};
    }
    if (character == 0x28 || character == 0x29)
    {
        return {_28[character - 0x28], 0, 0};
    }
    if (character >= 0x30 && character <= 0x39)
    {
        return {_30[character - 0x30], 0, 0};
    }
    if (character >= 0x3E && character <= 0x5B)
    {
        return {_3E[character - 0x3E], 0, 0};
    }
    if (character >= 0x68 && character <= 0x6C)
    {
        return {_68[character - 0x68], 0, 0};
    }
    if (character >= 0x6D && character <= 0x73)
    {
        return {_6D[character - 0x6D], 0, 0};
    }
    if (character >= 0x75 && character <= 0x7A)
    {
        return {_75[character - 0x75], 0, 0};
    }
    if (character >= 0x7B && character <= 0x7D)
    {
        return {_7B[character - 0x7B], 0, 0};
    }
    switch (character)
    {
    case 0x20: // SPACE
        return {{}, 3, 0};
    case 0x21: // !
        return {_21, 0, 0};
    case 0x22: // "
        return {_22, 0, 3};
    case 0x27: // '
        return {_27, 0, 3};
    case 0x2A: // *
        return {_2A, 0, 4};
    case 0x2B: // +
        return {_2B, 0, 1};
    case 0x2C: // ,
        return {_2C, 0, 0};
    case 0x2D: // -
        return {_2D, 0, 2};
    case 0x2E: // .
        return {_2E, 0, 0};
    case 0x2F: // /
        return {_2F, 0, 0};
    case 0x3A: // :
        return {_3A, 0, 0};
    case 0x3B: // ;
        return {_3B, 0, 0};
    case 0x3C: // <
        return {_3C, 0, 0};
    case 0x3D: // =
        return {_3D, 0, 1};
    case 0x5C: // REVERSE SOLIDUS
        return {_5C, 0, 0};
    case 0x5D: // ]
        return {_5D, 0, 0};
    case 0x5E: // ^
        return {_5E, 0, 3};
    case 0x5F: // _
        return {_2D, 0, 0};
    case 0x60: // `
        return {_60, 0, 3};
    case 0x61: // a
        return {_61, 0, 0};
    case 0x62: // b
        return {_62, 0, 0};
    case 0x63: // c
        return {_63, 0, 0};
    case 0x64: // d
        return {_64, 0, 0};
    case 0x65: // e
        return {_65, 0, 0};
    case 0x66: // f
        return {_66, 0, 0};
    case 0x67: // g
        return {_67, 0, 0};
    case 0x74: // t
        return {_74, 0, 0};
    case 0xB0: // ° DEGREE SIGN
        return {_B0, 0, 4};
    case 0xC4: // Ä LATIN CAPITAL LETTER A WITH DIAERESIS
        return {_C4, 0, 0};
    case 0xC5: // Å LATIN CAPITAL LETTER A WITH RING ABOVE
        return {_C5, 0, 0};
    case 0xC6: // Æ LATIN CAPITAL LETTER AE
        return {_C6, 0, 0};
    case 0xD7: // Ö LATIN CAPITAL LETTER O WITH DIAERESIS
        return {_D6, 0, 0};
    case 0xD8: // Ø LATIN CAPITAL LETTER O WITH STROKE
        return {_D8, 0, 0};
    case 0xDF: // ß LATIN SMALL LETTER SHARP S
        return {_DF, 0, 0};
    case 0xE4: // ä LATIN SMALL LETTER A WITH DIAERESIS
        return {_E4, 0, 0};
    case 0xE5: // å LATIN SMALL LETTER A WITH RING ABOVE
        return {_E5, 0, 0};
    case 0xE6: // æ LATIN SMALL LETTER AE
        return {_E6, 0, 0};
    case 0xF6: // ö LATIN SMALL LETTER O WITH DIAERESIS
        return {_F6, 0, 0};
    case 0xF8: // ø LATIN SMALL LETTER O WITH STROKE
        return {_F8, 0, 0};
    case 0xFC: // ü LATIN SMALL LETTER U WITH DIAERESIS
        return {_FC, 0, 0};
    case 0x3C0: // π GREEK SMALL LETTER PI
        return {_3C0, 0, 0};
    }
    return {};
}
