#include "fonts/LargeFont.h"

LargeFont *FontLarge = nullptr;

LargeFont::LargeFont() : FontModule("Large")
{
    FontLarge = this;
}

FontModule::Symbol LargeFont::getChar(wchar_t character)
{
    if (character >= 0x20 && character <= 0x7F && character < ascii.size() + 0x20)
    {
        return ascii[character - 0x20];
    }
    return {};
}
