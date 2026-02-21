#include "fonts/MediumRegularFont.h"

MediumRegularFont *FontMediumRegular = nullptr;

MediumRegularFont::MediumRegularFont() : FontModule("Medium regular") { FontMediumRegular = this; }

FontModule::Symbol MediumRegularFont::getChar(uint32_t character) const
{
    if (character >= 0x20 && character <= 0x7F && character < ascii.size() + 0x20)
    {
        return ascii[character - 0x20];
    }
    return {};
}
