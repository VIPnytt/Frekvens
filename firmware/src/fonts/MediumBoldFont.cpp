#include "fonts/MediumBoldFont.h"
#include "fonts/MediumFont.h"

MediumBoldFont *FontMediumBold = nullptr;

MediumBoldFont::MediumBoldFont() : FontModule("Medium bold")
{
    FontMediumBold = this;
}

FontModule::Symbol MediumBoldFont::getChar(uint32_t character)
{
    if (character >= 0x20 && character <= 0x7F && character < ascii.size() + 0x20)
    {
        return ascii[character - 0x20];
    }
    return FontMedium->getChar(character);
}
