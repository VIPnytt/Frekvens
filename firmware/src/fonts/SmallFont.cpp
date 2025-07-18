#include "fonts/SmallFont.h"

SmallFont *FontSmall = nullptr;

SmallFont::SmallFont() : FontModule("Small")
{
    FontSmall = this;
}

FontModule::Symbol SmallFont::getChar(wchar_t character)
{
    if (character >= 0x20 && character <= 0x7F && character < ascii.size() + 0x20)
    {
        return ascii[character - 0x20];
    }
    else if (character >= 0xC280 && character <= 0xDFBF)
    {
        for (const SymbolExtended extended : unicode)
        {
            if (extended.hex == character)
            {
                return extended.symbol;
            }
        }
    }
    return {};
}
