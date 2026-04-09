#include "fonts/MiniFont.h"

FontModule::Symbol MiniFont::getChar(uint32_t character) const
{
    if (character >= 0x30 && character <= 0x39)
    {
        return {chars30[character - 0x30], 0, 0};
    }
    if (character >= 0x41 && character <= 0x5D)
    {
        return {chars41[character - 0x41], 0, 0};
    }
    if (character >= 0x65 && character <= 0x67)
    {
        return {chars65[character - 0x65], 0, 0};
    }
    if (character == 0x68 || character == 0x69)
    {
        return {chars68[character - 0x68], 0, 0};
    }
    if (character == 0x6B || character == 0x6C)
    {
        return {chars6B[character - 0x6B], 0, 0};
    }
    if (character == 0x6D || character == 0x6E)
    {
        return {chars6D[character - 0x6D], 0, 0};
    }
    if (character >= 0x6F && character <= 0x72)
    {
        return {chars6F[character - 0x6F], 0, 0};
    }
    if (character >= 0x75 && character <= 0x78)
    {
        return {chars75[character - 0x75], 0, 0};
    }
    switch (character)
    {
    case 0x20: // SPACE
        return {{}, 3, 0};
    case 0x2B: // +
        return {char2B, 0, 1};
    case 0x2C: // ,
        return {char2C, 0, 0};
    case 0x2D: // -
        return {char2D, 0, 2};
    case 0x2E: // .
        return {char2E, 0, 0};
    case 0x3A: // :
        return {char3A, 0, 1};
    case 0x3D: // =
        return {char3D, 0, 1};
    case 0x5F: // _
        return {char2D, 0, 0};
    case 0x61: // a
        return {char61, 0, 0};
    case 0x62: // b
        return {char62, 0, 0};
    case 0x63: // c
        return {char63, 0, 0};
    case 0x64: // d
        return {char64, 0, 0};
    case 0x6A: // j
        return {char6A, 0, 0};
    case 0x73: // s
        return {char73, 0, 0};
    case 0x74: // t
        return {char74, 0, 0};
    case 0x79: // y
        return {char79, 0, 0};
    case 0x7A: // z
        return {char7A, 0, 0};
    case 0x7C: // |
        return {char7C, 0, 0};
    case 0xB0: // ° DEGREE SIGN
        return {charB0, 0, 2};
    case 0x3C0: // π GREEK SMALL LETTER PI
        return {char3C0, 0, 0};
    }
    return {};
}
