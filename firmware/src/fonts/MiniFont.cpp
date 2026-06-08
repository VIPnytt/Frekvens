#if FONT_MINI

#include "fonts/MiniFont.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS >= 5U, __STRING(FONT_MINI) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 5U, __STRING(FONT_MINI) " is not compatible with this device's display size.");

FontModule::Symbol MiniFont::getChar(char32_t character) const
{
    if (character >= '0' && character <= '9')
    {
        // U+0030-U+0039
        return toSymbol(digitZero_digitNine[character - '0']);
    }
    if (character >= 'A' && character <= ']')
    {
        // U+0041-U+005D
        return toSymbol(latinCapitalLetterA_rightSquareBracket[character - 'A']);
    }
    if (character >= 'e' && character <= 'g')
    {
        // U+0065-U+0067
        return toSymbol(latinSmallLetterE_latinSmallLetterG[character - 'e']);
    }
    if (character == 'h' || character == 'i')
    {
        // U+0068-U+0069
        return toSymbol(latinSmallLetterH_latinSmallLetterI[character - 'h']);
    }
    if (character == 'k' || character == 'l')
    {
        // U+006B-U+006C
        return toSymbol(latinSmallLetterK_latinSmallLetterL[character - 'k']);
    }
    if (character == 'm' || character == 'n')
    {
        // U+006D-U+006E
        return toSymbol(latinSmallLetterM_latinSmallLetterN[character - 'm']);
    }
    if (character >= 'o' && character <= 'r')
    {
        // U+006F-U+0072
        return toSymbol(latinSmallLetterO_latinSmallLetterR[character - 'o']);
    }
    if (character >= 'u' && character <= 'x')
    {
        // U+0075-U+0078
        return toSymbol(latinSmallLetterU_latinSmallLetterX[character - 'u']);
    }
    // NOLINTBEGIN(bugprone-branch-clone)
    switch (character)
    {
    case ' ': // U+0020 SPACE
        return whitespace(3U);
    case '+': // U+002B PLUS SIGN
        return toSymbol(plusSign, 1);
    case ',': // U+002C COMMA
        return toSymbol(comma);
    case '-': // U+002D HYPHEN-MINUS
        return toSymbol(hyphenMinus, 2);
    case '.': // U+002E FULL STOP
        return toSymbol(fullStop);
    case ':': // U+003A COLON
        return toSymbol(colon, 1);
    case '=': // U+003D EQUALS SIGN
        return toSymbol(equalsSign, 1);
    case '_': // U+005F LOW LINE
        return toSymbol(hyphenMinus);
    case 'a': // U+0061 LATIN SMALL LETTER A
        return toSymbol(latinSmallLetterA);
    case 'b': // U+0062 LATIN SMALL LETTER B
        return toSymbol(latinSmallLetterB);
    case 'c': // U+0063 LATIN SMALL LETTER C
        return toSymbol(latinSmallLetterC);
    case 'd': // U+0064 LATIN SMALL LETTER D
        return toSymbol(latinSmallLetterD);
    case 'j': // U+006A LATIN SMALL LETTER J
        return toSymbol(latinSmallLetterJ);
    case 's': // U+0073 LATIN SMALL LETTER S
        return toSymbol(latinSmallLetterS);
    case 't': // U+0074 LATIN SMALL LETTER T
        return toSymbol(latinSmallLetterT);
    case 'y': // U+0079 LATIN SMALL LETTER Y
        return toSymbol(latinSmallLetterY);
    case 'z': // U+007A LATIN SMALL LETTER Z
        return toSymbol(latinSmallLetterZ);
    case '|': // U+007C VERTICAL LINE
        return toSymbol(verticalLine);
    case U'°': // U+00B0 DEGREE SIGN
        return toSymbol(degreeSign, 3);
    case U'π': // U+03C0 GREEK SMALL LETTER PI
        return toSymbol(greekSmallLetterPi);
    default:
        return {};
    }
    // NOLINTEND(bugprone-branch-clone)
}

#endif // FONT_MINI
