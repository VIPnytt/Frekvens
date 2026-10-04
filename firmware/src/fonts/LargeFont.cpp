#include "fonts/LargeFont.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)

/**
 * @brief Converts a supported character to its large-font symbol.
 *
 * @param character Character to convert.
 * @return FontModule::Symbol The corresponding glyph, six-unit whitespace for a space, or an empty symbol when
 * unsupported.
 */
FontModule::Symbol LargeFont::getChar(char32_t character) const
{
    switch (character)
    {
    case ' ': // U+0020 SPACE
        return whitespace(6U);
    case '!': // U+0021 EXCLAMATION MARK
        return toSymbol(exclamationMark);
    case 'I': // U+0049 LATIN CAPITAL LETTER I
        return toSymbol(latinCapitalLetterI);
    case 'R': // U+0052 LATIN CAPITAL LETTER R
        return toSymbol(latinCapitalLetterR);
    case 'U': // U+0055 LATIN CAPITAL LETTER U
        return toSymbol(latinCapitalLetterU);
    case U'π': // U+03C0 GREEK SMALL LETTER PI
        return toSymbol(greekSmallLetterPi);
    default:
        return {};
    }
}
