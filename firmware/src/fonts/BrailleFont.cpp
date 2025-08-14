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
        return character >= 0x61 && character <= 0x7A && ascii[character - 0x20].bitmap.empty()
                   ? ascii[character - 0x40]
                   : ascii[character - 0x20];
    }
    return {};
}
