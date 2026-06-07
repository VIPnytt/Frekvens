#if FONT_LARGE

#include "fonts/LargeFont.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS >= 8U, __STRING(FONT_LARGE) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 8U, __STRING(FONT_LARGE) " is not compatible with this device's display size.");

FontModule::Symbol LargeFont::getChar(char32_t character) const
{
    // NOLINTBEGIN(bugprone-branch-clone)
    switch (character)
    {
    case ' ': // SPACE
        return whitespace(6U);
    case '!': // EXCLAMATION MARK
        return toSymbol(exclamationMark);
    case 'I': // LATIN CAPITAL LETTER I
        return toSymbol(latinCapitalLetterI);
    case 'R': // LATIN CAPITAL LETTER R
        return toSymbol(latinCapitalLetterR);
    case 'U': // LATIN CAPITAL LETTER U
        return toSymbol(latinCapitalLetterU);
    case U'π': // GREEK SMALL LETTER PI
        return toSymbol(greekSmallLetterPi);
    default:
        return {};
    }
    // NOLINTEND(bugprone-branch-clone)
}

#endif // FONT_LARGE
