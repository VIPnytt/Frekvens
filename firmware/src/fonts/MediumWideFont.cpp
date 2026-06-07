#if FONT_MEDIUMWIDE

#include "fonts/MediumWideFont.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS >= 7U, __STRING(FONT_MEDIUMWIDE) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 7U, __STRING(FONT_MEDIUMWIDE) " is not compatible with this device's display size.");

FontModule::Symbol MediumWideFont::getChar(char32_t character) const
{
    if (character >= '0' && character <= '9')
    {
        return toSymbol(digitZero_digitNine[character - '0']);
    }
    // NOLINTBEGIN(bugprone-branch-clone)
    switch (character)
    {
    case ' ': // SPACE
        return whitespace(6U);
    case 'I': // LATIN CAPITAL LETTER I
        return toSymbol(latinCapitalLetterI);
    case 'O': // LATIN CAPITAL LETTER O
        return toSymbol(latinCapitalLetterO);
    case 'o': // LATIN SMALL LETTER O
        return toSymbol(latinSmallLetterO);
    default:
        return {};
    }
    // NOLINTEND(bugprone-branch-clone)
}

#endif // FONT_MEDIUMWIDE
