#if FONT_LARGE

#include "fonts/LargeFont.h"

FontModule::Symbol LargeFont::getChar(uint32_t character) const
{
    // NOLINTBEGIN(bugprone-branch-clone)
    switch (character)
    {
    case 0x20: // SPACE
        return whitespace(6);
    case 0x21: // !
        return toSymbol(char21);
    case 0x49: // I
        return toSymbol(char49);
    case 0x52: // R
        return toSymbol(char52);
    case 0x55: // U
        return toSymbol(char55);
    case 0x3C0: // π GREEK SMALL LETTER PI
        return toSymbol(char3C0);
    }
    // NOLINTEND(bugprone-branch-clone)
    return {};
}

#endif // FONT_LARGE
