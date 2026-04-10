#include "fonts/MediumWideFont.h"

FontModule::Symbol MediumWideFont::getChar(uint32_t character) const
{
    if (character >= 0x30 && character <= 0x39)
    {
        return toSymbol(chars30[character - 0x30]);
    }
    switch (character)
    {
    case 0x20: // SPACE
        return whitespace(6);
    case 0x49: // I
        return toSymbol(char49);
    case 0x4F: // O
        return toSymbol(char4F);
    case 0x6F: // o
        return toSymbol(char6F);
    }
    return {};
}
