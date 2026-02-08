#if FONT_BRAILLE

#include "fonts/BrailleFont.h"

BrailleFont *FontBraille = nullptr;

BrailleFont::BrailleFont() : FontModule("Braille") { FontBraille = this; }

FontModule::Symbol BrailleFont::getChar(uint32_t character) const
{
    if (character >= 0x20 && character <= 0x5A)
    {
        return ascii[character - 0x20];
    }
    else if (character >= 0x61 && character <= 0x7A)
    {
        return ascii[character - 0x40];
    }
    return {};
}

#endif // FONT_BRAILLE
