#include "fonts/MediumFont.h"

MediumFont *FontMedium = nullptr; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

MediumFont::MediumFont() : FontModule("Medium") { FontMedium = this; }

FontModule::Symbol MediumFont::getChar(uint32_t character) const
{
    if (character >= 0x20 && character <= 0x7F && character < ascii.size() + 0x20)
    {
        return ascii[character - 0x20];
    }
    return {};
}
