#if FONT_MEDIUMBOLD

#include "fonts/MediumBoldFont.h"

FontModule::Symbol MediumBoldFont::getChar(uint32_t character) const
{
    if (character >= 0x30 && character <= 0x39)
    {
        return toSymbol(chars30[character - 0x30]);
    }
    // NOLINTBEGIN(bugprone-branch-clone)
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
    // NOLINTEND(bugprone-branch-clone)
    return {};
}

#endif // FONT_MEDIUMBOLD
