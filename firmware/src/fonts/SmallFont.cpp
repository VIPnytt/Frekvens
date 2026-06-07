#if FONT_SMALL

#include "fonts/SmallFont.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS >= 7U, __STRING(FONT_SMALL) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 7U, __STRING(FONT_SMALL) " is not compatible with this device's display size.");

FontModule::Symbol SmallFont::getChar(char32_t character) const
{
    if (character >= '#' && character <= '&')
    {
        return toSymbol(numberSign_ampersand[character - '#']);
    }
    if (character == '(' || character == ')')
    {
        return toSymbol(leftParenthesis_rightParenthesis[character - '(']);
    }
    if (character >= '0' && character <= '9')
    {
        return toSymbol(digitZero_digitNine[character - '0']);
    }
    if (character >= '>' && character <= 'X')
    {
        return toSymbol(greaterThanSign_leftSquareBracket[character - '>']);
    }
    if (character >= 'h' && character <= 'l')
    {
        return toSymbol(latinSmallLetterH_latinSmallLetterL[character - 'h']);
    }
    if (character >= 'm' && character <= 's')
    {
        return toSymbol(latinSmallLetterM_latinSmallLetterS[character - 'm']);
    }
    if (character >= 'u' && character <= 'z')
    {
        return toSymbol(latinSmallLetterU_latinSmallLetterZ[character - 'u']);
    }
    if (character >= '{' && character <= '}')
    {
        return toSymbol(leftCurlyBracket_rightCurlyBracket[character - '{']);
    }
    // NOLINTBEGIN(bugprone-branch-clone)
    switch (character)
    {
    case ' ': // SPACE
        return whitespace(3);
    case '!': // EXCLAMATION MARK
        return toSymbol(exclamationMark);
    case '"': // QUOTATION MARK
        return toSymbol(quotationMark, 3);
    case '\'': // APOSTROPHE
        return toSymbol(apostrophe, 3);
    case '*': // ASTERISK
        return toSymbol(asterisk, 4);
    case '+': // PLUS SIGN
        return toSymbol(plusSign, 1);
    case ',': // COMMA
        return toSymbol(comma);
    case '-': // HYPHEN-MINUS
        return toSymbol(hyphenMinus, 2);
    case '.': // FULL STOP
        return toSymbol(fullStop);
    case '/': // SOLIDUS
        return toSymbol(solidus);
    case ':': // COLON
        return toSymbol(colon);
    case ';': // SEMICOLON
        return toSymbol(semicolon);
    case '<': // LESS-THAN SIGN
        return toSymbol(lessThanSign);
    case '=': // EQUALS SIGN
        return toSymbol(equalsSign, 1);
    case '\\': // REVERSE SOLIDUS
        return toSymbol(reverseSolidus);
    case ']': // RIGHT SQUARE BRACKET
        return toSymbol(rightSquareBracket);
    case '^': // CIRCUMFLEX ACCENT
        return toSymbol(circumflexAccent, 3);
    case '_': // LOW LINE
        return toSymbol(hyphenMinus);
    case '`': // GRAVE ACCENT
        return toSymbol(graveAccent, 3);
    case 'a': // LATIN SMALL LETTER A
        return toSymbol(latinSmallLetterA);
    case 'b': // LATIN SMALL LETTER B
        return toSymbol(latinSmallLetterB);
    case 'c': // LATIN SMALL LETTER C
        return toSymbol(latinSmallLetterC);
    case 'd': // LATIN SMALL LETTER D
        return toSymbol(latinSmallLetterD);
    case 'e': // LATIN SMALL LETTER E
        return toSymbol(latinSmallLetterE);
    case 'f': // LATIN SMALL LETTER F
        return toSymbol(latinSmallLetterF);
    case 'g': // LATIN SMALL LETTER G
        return toSymbol(latinSmallLetterG);
    case 't': // LATIN SMALL LETTER T
        return toSymbol(latinSmallLetterT);
    case U'°': // DEGREE SIGN
        return toSymbol(degreeSign, 4);
    case U'Ä': // LATIN CAPITAL LETTER A WITH DIAERESIS
        return toSymbol(latinCapitalLetterAWithDiaeresis);
    case U'Å': // LATIN CAPITAL LETTER A WITH RING ABOVE
        return toSymbol(latinCapitalLetterAWithRingAbove);
    case U'Æ': // LATIN CAPITAL LETTER AE
        return toSymbol(latinCapitalLetterAe);
    case U'Ö': // LATIN CAPITAL LETTER O WITH DIAERESIS
        return toSymbol(latinCapitalLetterOWithDiaeresis);
    case U'Ø': // LATIN CAPITAL LETTER O WITH STROKE
        return toSymbol(latinCapitalLetterOWithStroke);
    case U'ß': // LATIN SMALL LETTER SHARP S
        return toSymbol(latinSmallLetterSharpS);
    case U'ä': // LATIN SMALL LETTER A WITH DIAERESIS
        return toSymbol(latinSmallLetterAWithDiaeresis);
    case U'å': // LATIN SMALL LETTER A WITH RING ABOVE
        return toSymbol(latinSmallLetterAWithRingAbove);
    case U'æ': // LATIN SMALL LETTER AE
        return toSymbol(latinSmallLetterAe);
    case U'ö': // LATIN SMALL LETTER O WITH DIAERESIS
        return toSymbol(latinSmallLetterOWithDiaeresis);
    case U'ø': // LATIN SMALL LETTER O WITH STROKE
        return toSymbol(latinSmallLetterOWithStroke);
    case U'ü': // LATIN SMALL LETTER U WITH DIAERESIS
        return toSymbol(latinSmallLetterUWithDiaeresis);
    case U'π': // GREEK SMALL LETTER PI
        return toSymbol(greekSmallLetterPi);
    default:
        return {};
    }
    // NOLINTEND(bugprone-branch-clone)
}

#endif // FONT_SMALL
