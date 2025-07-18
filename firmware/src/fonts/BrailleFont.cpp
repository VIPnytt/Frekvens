#include "fonts/BrailleFont.h"

BrailleFont *FontBraille = nullptr;

BrailleFont::BrailleFont() : FontModule("Braille")
{
    FontBraille = this;
}

FontModule::Symbol BrailleFont::getChar(wchar_t character)
{
    if (character >= 0x20 && character <= 0x7F && character < ascii.size() + 0x20)
    {
        return ascii[character - 0x20];
    }
    return {};
}
