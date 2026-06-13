#if FONT_SMALL

#include "fonts/SmallFont.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS >= 7U, __STRING(FONT_SMALL) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 7U, __STRING(FONT_SMALL) " is not compatible with this device's display size.");

FontModule::Symbol SmallFont::getChar(char32_t character) const
{
    if (character >= '#' && character <= '&')
    {
        // U+0023-U+0026
        return toSymbol(numberSign_ampersand[character - '#']);
    }
    if (character == '(' || character == ')')
    {
        // U+0028-U+0029
        return toSymbol(leftParenthesis_rightParenthesis[character - '(']);
    }
    if (character >= '0' && character <= '9')
    {
        // U+0030-U+0039
        return toSymbol(digitZero_digitNine[character - '0']);
    }
    if (character >= '>' && character <= 'X')
    {
        // U+003E-U+0058
        return toSymbol(greaterThanSign_leftSquareBracket[character - '>']);
    }
    if (character >= 'h' && character <= 'l')
    {
        // U+0068-U+006C
        return toSymbol(latinSmallLetterH_latinSmallLetterL[character - 'h']);
    }
    if (character >= 'm' && character <= 's')
    {
        // U+006D-U+0073
        return toSymbol(latinSmallLetterM_latinSmallLetterS[character - 'm']);
    }
    if (character >= 'u' && character <= 'z')
    {
        // U+0075-U+007A
        return toSymbol(latinSmallLetterU_latinSmallLetterZ[character - 'u']);
    }
    if (character >= '{' && character <= '}')
    {
        // U+007B-U+007D
        return toSymbol(leftCurlyBracket_rightCurlyBracket[character - '{']);
    }
    // NOLINTBEGIN(bugprone-branch-clone)
    switch (character)
    {
    case ' ': // U+0020 SPACE
        return whitespace(3);
    case '!': // U+0021 EXCLAMATION MARK
        return toSymbol(exclamationMark);
    case '"': // U+0022 QUOTATION MARK
        return toSymbol(quotationMark, 3);
    case '\'': // U+0027 APOSTROPHE
        return toSymbol(apostrophe, 3);
    case '*': // U+002A ASTERISK
        return toSymbol(asterisk, 4);
    case '+': // U+002B PLUS SIGN
        return toSymbol(plusSign, 1);
    case ',': // U+002C COMMA
        return toSymbol(comma);
    case '-': // U+002D HYPHEN-MINUS
        return toSymbol(hyphenMinus, 2);
    case '.': // U+002E FULL STOP
        return toSymbol(fullStop);
    case '/': // U+002F SOLIDUS
        return toSymbol(solidus);
    case ':': // U+003A COLON
        return toSymbol(colon);
    case ';': // U+003B SEMICOLON
        return toSymbol(semicolon);
    case '<': // U+003C LESS-THAN SIGN
        return toSymbol(lessThanSign);
    case '=': // U+003D EQUALS SIGN
        return toSymbol(equalsSign, 1);
    case '\\': // U+005C REVERSE SOLIDUS
        return toSymbol(reverseSolidus);
    case ']': // U+005D RIGHT SQUARE BRACKET
        return toSymbol(rightSquareBracket);
    case '^': // U+005E CIRCUMFLEX ACCENT
        return toSymbol(circumflexAccent, 3);
    case '_': // U+005F LOW LINE
        return toSymbol(hyphenMinus);
    case '`': // U+0060 GRAVE ACCENT
        return toSymbol(graveAccent, 3);
    case 'a': // U+0061 LATIN SMALL LETTER A
        return toSymbol(latinSmallLetterA);
    case 'b': // U+0062 LATIN SMALL LETTER B
        return toSymbol(latinSmallLetterB);
    case 'c': // U+0063 LATIN SMALL LETTER C
        return toSymbol(latinSmallLetterC);
    case 'd': // U+0064 LATIN SMALL LETTER D
        return toSymbol(latinSmallLetterD);
    case 'e': // U+0065 LATIN SMALL LETTER E
        return toSymbol(latinSmallLetterE);
    case 'f': // U+0066 LATIN SMALL LETTER F
        return toSymbol(latinSmallLetterF);
    case 'g': // U+0067 LATIN SMALL LETTER G
        return toSymbol(latinSmallLetterG);
    case 't': // U+0074 LATIN SMALL LETTER T
        return toSymbol(latinSmallLetterT);
    case U'°': // U+00B0 DEGREE SIGN
        return toSymbol(degreeSign, 4);
    case U'Ä': // U+00C4 LATIN CAPITAL LETTER A WITH DIAERESIS
        return toSymbol(latinCapitalLetterAWithDiaeresis);
    case U'Å': // U+00C5 LATIN CAPITAL LETTER A WITH RING ABOVE
        return toSymbol(latinCapitalLetterAWithRingAbove);
    case U'Æ': // U+00C6 LATIN CAPITAL LETTER AE
        return toSymbol(latinCapitalLetterAe);
    case U'Ö': // U+00D6 LATIN CAPITAL LETTER O WITH DIAERESIS
        return toSymbol(latinCapitalLetterOWithDiaeresis);
    case U'Ø': // U+00D8 LATIN CAPITAL LETTER O WITH STROKE
        return toSymbol(latinCapitalLetterOWithStroke);
    case U'ß': // U+00DF LATIN SMALL LETTER SHARP S
        return toSymbol(latinSmallLetterSharpS);
    case U'ä': // U+00E4 LATIN SMALL LETTER A WITH DIAERESIS
        return toSymbol(latinSmallLetterAWithDiaeresis);
    case U'å': // U+00E5 LATIN SMALL LETTER A WITH RING ABOVE
        return toSymbol(latinSmallLetterAWithRingAbove);
    case U'æ': // U+00E6 LATIN SMALL LETTER AE
        return toSymbol(latinSmallLetterAe);
    case U'ö': // U+00F6 LATIN SMALL LETTER O WITH DIAERESIS
        return toSymbol(latinSmallLetterOWithDiaeresis);
    case U'ø': // U+00F8 LATIN SMALL LETTER O WITH STROKE
        return toSymbol(latinSmallLetterOWithStroke);
    case U'ü': // U+00FC LATIN SMALL LETTER U WITH DIAERESIS
        return toSymbol(latinSmallLetterUWithDiaeresis);
    case U'π': // U+03C0 GREEK SMALL LETTER PI
        return toSymbol(greekSmallLetterPi);
    default:
        return {};
    }
    // NOLINTEND(bugprone-branch-clone)
}

#endif // FONT_SMALL
