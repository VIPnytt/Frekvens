#include "fonts/MiniFont.h"

MiniFont *FontMini = nullptr;

MiniFont::MiniFont() : FontModule("Mini")
{
    FontMini = this;
}

FontModule::Symbol MiniFont::getChar(uint32_t character) const
{
    if (character >= 0x20 && character <= 0x7F && character < ascii.size() + 0x20)
    {
        return ascii[character - 0x20];
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
