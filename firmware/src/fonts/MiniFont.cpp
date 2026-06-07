#if FONT_MINI

#include "fonts/MiniFont.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS >= 5U, __STRING(FONT_MINI) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 5U, __STRING(FONT_MINI) " is not compatible with this device's display size.");

FontModule::Symbol MiniFont::getChar(char32_t character) const
{
    if (character >= '0' && character <= '9')
    {
        return toSymbol(digitZero_digitNine[character - '0']);
    }
    if (character >= 'A' && character <= ']')
    {
        return toSymbol(latinCapitalLetterA_rightSquareBracket[character - 'A']);
    }
    if (character >= 'e' && character <= 'g')
    {
        return toSymbol(latinSmallLetterE_latinSmallLetterG[character - 'e']);
    }
    if (character == 'h' || character == 'i')
    {
        return toSymbol(latinSmallLetterH_latinSmallLetterI[character - 'h']);
    }
    if (character == 'k' || character == 'l')
    {
        return toSymbol(latinSmallLetterK_latinSmallLetterL[character - 'k']);
    }
    if (character == 'm' || character == 'n')
    {
        return toSymbol(latinSmallLetterM_latinSmallLetterN[character - 'm']);
    }
    if (character >= 'o' && character <= 'r')
    {
        return toSymbol(latinSmallLetterO_latinSmallLetterR[character - 'o']);
    }
    if (character >= 'u' && character <= 'x')
    {
        return toSymbol(latinSmallLetterU_latinSmallLetterX[character - 'u']);
    }
    // NOLINTBEGIN(bugprone-branch-clone)
    switch (character)
    {
    case ' ': // SPACE
        return whitespace(3U);
    case '+': // PLUS SIGN
        return toSymbol(plusSign, 1);
    case ',': // COMMA
        return toSymbol(comma);
    case '-': // HYPHEN-MINUS
        return toSymbol(hyphenMinus, 2);
    case '.': // FULL STOP
        return toSymbol(fullStop);
    case ':': // COLON
        return toSymbol(colon, 1);
    case '=': // EQUALS SIGN
        return toSymbol(equalsSign, 1);
    case '_': // LOW LINE
        return toSymbol(hyphenMinus);
    case 'a': // LATIN SMALL LETTER A
        return toSymbol(latinSmallLetterA);
    case 'b': // LATIN SMALL LETTER B
        return toSymbol(latinSmallLetterB);
    case 'c': // LATIN SMALL LETTER C
        return toSymbol(latinSmallLetterC);
    case 'd': // LATIN SMALL LETTER D
        return toSymbol(latinSmallLetterD);
    case 'j': // LATIN SMALL LETTER J
        return toSymbol(latinSmallLetterJ);
    case 's': // LATIN SMALL LETTER S
        return toSymbol(latinSmallLetterS);
    case 't': // LATIN SMALL LETTER T
        return toSymbol(latinSmallLetterT);
    case 'y': // LATIN SMALL LETTER Y
        return toSymbol(latinSmallLetterY);
    case 'z': // LATIN SMALL LETTER Z
        return toSymbol(latinSmallLetterZ);
    case '|': // VERTICAL LINE
        return toSymbol(verticalLine);
    case U'°': // DEGREE SIGN
        return toSymbol(degreeSign, 3);
    case U'π': // GREEK SMALL LETTER PI
        return toSymbol(greekSmallLetterPi);
    default:
        return {};
    }
    // NOLINTEND(bugprone-branch-clone)
}

#endif // FONT_MINI
