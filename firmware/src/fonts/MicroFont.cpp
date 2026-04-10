#include "fonts/MicroFont.h"

FontModule::Symbol MicroFont::getChar(uint32_t character) const
{
    if (character == 0x28 || character == 0x29)
    {
        return toSymbol(chars28[character - 0x28]);
    }
    if (character >= 0x2F && character <= 0x3E)
    {
        return toSymbol(chars2F[character - 0x2F]);
    }
    if (character >= 0x41 && character <= 0x5A)
    {
        return toSymbol(chars41[character - 0x41]);
    }
    if (character >= 0x5B && character <= 0x5D)
    {
        return toSymbol(chars5B[character - 0x5B]);
    }
    if (character >= 0x61 && character <= 0x7A)
    {
        return toSymbol(chars41[character - 0x61]);
    }
    if (character >= 0x7B && character <= 0x7D)
    {
        return toSymbol(chars7B[character - 0x7B]);
    }
    switch (character)
    {
    case 0x20: // SPACE
        return whitespace(3);
    case 0x22: // "
        return toSymbol(char22, 0, 2);
    case 0x27: // '
    case 0x2A: // *
    case 0x60: // `
    case 0xB0: // ° DEGREE SIGN
        return toSymbol(char27, 0, 2);
    case 0x2B: // +
        return toSymbol(char2B);
    case 0x2C: // ,
    case 0x2E: // .
        return toSymbol(char27);
    case 0x2D: // -
        return toSymbol(char2D, 0, 1);
    case 0x5E: // ^
        return toSymbol(char5E, 0, 1);
    case 0x5F: // _
        return toSymbol(char2D);
    }
    return {};
}
