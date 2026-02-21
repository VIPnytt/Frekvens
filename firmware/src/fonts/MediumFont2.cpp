#include "fonts/MediumFont2.h"

MediumFont2 *FontMedium2 = nullptr;

MediumFont2::MediumFont2() : FontModule("Medium2") { FontMedium2 = this; }

FontModule::Symbol MediumFont2::getChar(uint32_t character) const
{
    if (character >= 0x20 && character <= 0x7F && character < ascii.size() + 0x20)
    {
        return ascii[character - 0x20];
    }
    return {};
}
