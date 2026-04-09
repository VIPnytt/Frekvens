#include "fonts/MicroFont.h"

FontModule::Symbol MicroFont::getChar(uint32_t character) const
{
    if (character == 0x28 || character == 0x29)
    {
        return {chars28[character - 0x28], 0, 0};
    }
    if (character >= 0x2F && character <= 0x3E)
    {
        return {chars2F[character - 0x2F], 0, 0};
    }
    if (character >= 0x41 && character <= 0x5A)
    {
        return {chars41[character - 0x41], 0, 0};
    }
    if (character >= 0x5B && character <= 0x5D)
    {
        return {chars5B[character - 0x5B], 0, 0};
    }
    if (character >= 0x61 && character <= 0x7A)
    {
        return {chars41[character - 0x61], 0, 0};
    }
    if (character >= 0x7B && character <= 0x7D)
    {
        return {chars7B[character - 0x7B], 0, 0};
    }
    switch (character)
    {
    case 0x20: // SPACE
        return {{}, 3, 0};
    case 0x22: // "
        return {char22, 0, 2};
    case 0x27: // '
    case 0x2A: // *
    case 0x60: // `
    case 0xB0: // ° DEGREE SIGN
        return {char27, 0, 2};
    case 0x2B: // +
        return {char2B, 0, 0};
    case 0x2C: // ,
    case 0x2E: // .
        return {char27, 0, 0};
    case 0x2D: // -
        return {char2D, 0, 1};
    case 0x5E: // ^
        return {char5E, 0, 1};
    case 0x5F: // _
        return {char2D, 0, 0};
    }
    return {};
}
