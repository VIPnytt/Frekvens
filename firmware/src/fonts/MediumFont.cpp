#include "fonts/MediumFont.h"

MediumFont *FontMedium = nullptr;

MediumFont::MediumFont() : FontModule("Medium")
{
    FontMedium = this;
}

FontModule::Symbol MediumFont::getChar(uint32_t character)
{
    if (character >= 0x20 && character <= 0x7F && character < ascii.size() + 0x20)
    {
        return ascii[character - 0x20];
    }
    return {};
}
