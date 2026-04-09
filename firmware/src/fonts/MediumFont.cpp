#include "fonts/MediumFont.h"

FontModule::Symbol MediumFont::getChar(uint32_t character) const
{
    if (character >= 0x30 && character <= 0x39)
    {
        return {_30[character - 0x30], 0, 0};
    }
    switch (character)
    {
    case 0x20: // SPACE
        return {{}, 6, 0};
    case 0x49: // I
        return {_49, 0, 0};
    case 0x4F: // O
        return {_4F, 0, 0};
    case 0x6F: // o
        return {_6F, 0, 0};
    }
    return {};
}
