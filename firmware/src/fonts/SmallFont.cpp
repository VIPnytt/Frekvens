#include "fonts/SmallFont.h"

SmallFont *FontSmall = nullptr;

SmallFont::SmallFont() : FontModule("Small") { FontSmall = this; }

FontModule::Symbol SmallFont::getChar(uint32_t character) const
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
