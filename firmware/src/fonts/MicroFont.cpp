#include "fonts/MicroFont.h"

MicroFont *FontMicro = nullptr;

MicroFont::MicroFont() : FontModule("Micro")
{
    FontMicro = this;
}

FontModule::Symbol MicroFont::getChar(uint32_t character) const
{
    if (character >= 0x20 && character <= 0x7F && character < ascii.size() + 0x20)
    {
        return character >= 0x61 && character <= 0x7A && ascii[character - 0x20].bitmap.empty()
                   ? ascii[character - 0x40]
                   : ascii[character - 0x20];
    }
    else if (character >= 0x80 && character <= 0x10FFFF)
    {
        for (const SymbolExtended &extended : unicode)
        {
            if (extended.hex == character)
            {
                return extended.symbol;
            }
        }
    }
    return {};
}
