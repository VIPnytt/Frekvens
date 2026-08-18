#if FONT_MEDIUMWIDE

#include "fonts/MediumWideFont.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS >= 7U, __STRING(FONT_MEDIUMWIDE) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 7U, __STRING(FONT_MEDIUMWIDE) " is not compatible with this device's display size.");

/**
 * @brief Converts a supported character to its medium-wide font symbol.
 *
 * @param character Character to convert.
 * @return Font symbol for a supported digit, space, or selected letter; an empty symbol otherwise.
 */
FontModule::Symbol MediumWideFont::getChar(char32_t character) const
{
    if (character >= '0' && character <= '9')
    {
        // U+0030-U+0039
        return toSymbol(digitZero_digitNine[character - '0']);
    }
    switch (character)
    {
    case ' ': // U+0020 SPACE
        return whitespace(6U);
    case 'I': // U+0049 LATIN CAPITAL LETTER I
        return toSymbol(latinCapitalLetterI);
    case 'O': // U+004F LATIN CAPITAL LETTER O
        return toSymbol(latinCapitalLetterO);
    case 'o': // U+006F LATIN SMALL LETTER O
        return toSymbol(latinSmallLetterO);
    default:
        return {};
    }
}

#endif // FONT_MEDIUMWIDE
