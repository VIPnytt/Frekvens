#include "fonts/LargeFont.h"

FontModule::Symbol LargeFont::getChar(uint32_t character) const
{
    switch (character)
    {
    case 0x20: // SPACE
        return {{}, 6, 0};
    case 0x21: // !
        return {_21, 0, 0};
    case 0x49: // I
        return {_49, 0, 0};
    case 0x52: // R
        return {_52, 0, 0};
    case 0x55: // U
        return {_55, 0, 0};
    case 0x3C0: // π GREEK SMALL LETTER PI
        return {_3C0, 0, 0};
    }
    return {};
}
