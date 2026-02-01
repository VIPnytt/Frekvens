#include "fonts/LargeFont.h"

LargeFont *FontLarge = nullptr;

LargeFont::LargeFont() : FontModule("Large") { FontLarge = this; }

FontModule::Symbol LargeFont::getChar(uint32_t character) const
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
