#if FONT_MEDIUMBOLD

#include "fonts/MediumBoldFont.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS >= 6U, __STRING(FONT_MEDIUMBOLD) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 7U, __STRING(FONT_MEDIUMBOLD) " is not compatible with this device's display size.");

/**
 * @brief Returns the glyph for a supported character in the medium-bold font.
 *
 * @param character Character to convert to a glyph.
 * @return FontModule::Symbol The corresponding glyph, or an empty symbol for unsupported characters.
 */
FontModule::Symbol MediumBoldFont::getChar(char32_t character) const
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

#endif // FONT_MEDIUMBOLD
