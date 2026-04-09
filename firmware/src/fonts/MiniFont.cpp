#include "fonts/MiniFont.h"

FontModule::Symbol MiniFont::getChar(uint32_t character) const
{
    if (character >= 0x30 && character <= 0x39)
    {
        return {_30[character - 0x30], 0, 0};
    }
    if (character >= 0x41 && character <= 0x5D)
    {
        return {_41[character - 0x41], 0, 0};
    }
    if (character >= 0x65 && character <= 0x67)
    {
        return {_65[character - 0x65], 0, 0};
    }
    if (character == 0x68 || character == 0x69)
    {
        return {_68[character - 0x68], 0, 0};
    }
    if (character == 0x6B || character == 0x6C)
    {
        return {_6B[character - 0x6B], 0, 0};
    }
    if (character == 0x6D || character == 0x6E)
    {
        return {_6D[character - 0x6D], 0, 0};
    }
    if (character >= 0x6F && character <= 0x72)
    {
        return {_6F[character - 0x6F], 0, 0};
    }
    if (character >= 0x75 && character <= 0x78)
    {
        return {_75[character - 0x75], 0, 0};
    }
    switch (character)
    {
    case 0x20: // SPACE
        return {{}, 3, 0};
    case 0x2B: // +
        return {_2B, 0, 1};
    case 0x2C: // ,
        return {_2C, 0, 0};
    case 0x2D: // -
        return {_2D, 0, 2};
    case 0x2E: // .
        return {_2E, 0, 0};
    case 0x3A: // :
        return {_3A, 0, 1};
    case 0x3D: // =
        return {_3D, 0, 1};
    case 0x5F: // _
        return {_2D, 0, 0};
    case 0x61: // a
        return {_61, 0, 0};
    case 0x62: // b
        return {_62, 0, 0};
    case 0x63: // c
        return {_63, 0, 0};
    case 0x64: // d
        return {_64, 0, 0};
    case 0x6A: // j
        return {_6A, 0, 0};
    case 0x73: // s
        return {_73, 0, 0};
    case 0x74: // t
        return {_74, 0, 0};
    case 0x79: // y
        return {_79, 0, 0};
    case 0x7A: // z
        return {_7A, 0, 0};
    case 0x7C: // |
        return {_7C, 0, 0};
    case 0xB0: // ° DEGREE SIGN
        return {_B0, 0, 2};
    case 0x3C0: // π GREEK SMALL LETTER PI
        return {_3C0, 0, 0};
    }
    return {};
}
