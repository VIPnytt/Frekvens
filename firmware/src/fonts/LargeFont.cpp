#include "fonts/LargeFont.h"

FontModule::Symbol LargeFont::getChar(uint32_t character) const
{
    switch (character)
    {
    case 0x20: // SPACE
        return {{}, 6, 0};
    case 0x21: // !
        return {char21, 0, 0};
    case 0x49: // I
        return {char49, 0, 0};
    case 0x52: // R
        return {char52, 0, 0};
    case 0x55: // U
        return {char55, 0, 0};
    case 0x3C0: // π GREEK SMALL LETTER PI
        return {char3C0, 0, 0};
    }
    return {};
}
