#include "fonts/MicroFont.h"

FontModule::Symbol MicroFont::getChar(uint32_t character) const
{
    if (character == 0x28 || character == 0x29)
    {
        return {_28[character - 0x28], 0, 0};
    }
    if (character >= 0x2F && character <= 0x3E)
    {
        return {_2F[character - 0x2F], 0, 0};
    }
    if (character >= 0x41 && character <= 0x5A)
    {
        return {letters[character - 0x41], 0, 0};
    }
    if (character >= 0x5B && character <= 0x5D)
    {
        return {_5B[character - 0x5B], 0, 0};
    }
    if (character >= 0x61 && character <= 0x7A)
    {
        return {letters[character - 0x61], 0, 0};
    }
    if (character >= 0x7B && character <= 0x7D)
    {
        return {_7B[character - 0x7B], 0, 0};
    }
    switch (character)
    {
    case 0x20: // SPACE
        return {{}, 3, 0};
    case 0x22: // "
        return {_22, 0, 2};
    case 0x27: // '
    case 0x2A: // *
    case 0x60: // `
    case 0xB0: // ° DEGREE SIGN
        return {_27, 0, 2};
    case 0x2B: // +
        return {_2B, 0, 0};
    case 0x2C: // ,
    case 0x2E: // .
        return {_27, 0, 0};
    case 0x2D: // -
        return {_2D, 0, 1};
    case 0x5E: // ^
        return {_5E, 0, 1};
    case 0x5F: // _
        return {_2D, 0, 0};
    }
    return {};
}
