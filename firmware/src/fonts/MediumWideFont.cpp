#include "fonts/MediumWideFont.h"

MediumWideFont *FontMediumWide = nullptr; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

MediumWideFont::MediumWideFont() : FontModule("Medium Wide") { FontMediumWide = this; }

FontModule::Symbol MediumWideFont::getChar(uint32_t character) const
{
    if (character >= 0x20 && character <= 0x7F && character < ascii.size() + 0x20)
    {
        return ascii[character - 0x20];
    }
    return {};
}
