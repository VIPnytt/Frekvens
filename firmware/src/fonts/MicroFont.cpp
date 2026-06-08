#if FONT_MICRO

#include "fonts/MicroFont.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS >= 3U, __STRING(FONT_MICRO) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 3U, __STRING(FONT_MICRO) " is not compatible with this device's display size.");

FontModule::Symbol MicroFont::getChar(char32_t character) const
{
    if (character == '(' || character == ')')
    {
        // U+0028-U+0029
        return toSymbol(leftParenthesis_rightParenthesis[character - '(']);
    }
    if (character >= '/' && character <= '>')
    {
        // U+002F-U+003E
        return toSymbol(solidus_greaterThanSign[character - '/']);
    }
    if (character >= 'A' && character <= 'Z')
    {
        // U+0041-U+005A
        return toSymbol(latinLetterA_latinLetterZ[character - 'A']);
    }
    if (character >= 'X' && character <= ']')
    {
        // U+0058-U+005D
        return toSymbol(leftSquareBracket_rightSquareBracket[character - 'X']);
    }
    if (character >= 'a' && character <= 'z')
    {
        // U+0061-U+007A
        return toSymbol(latinLetterA_latinLetterZ[character - 'a']);
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
        return whitespace(3U);
    case '"': // U+0022 QUOTATION MARK
        return toSymbol(quotationMark, 2);
    case '\'': // U+0027 APOSTROPHE
    case '*':  // U+002A ASTERISK
    case '`':  // U+0060 GRAVE ACCENT
    case U'°': // U+00B0 DEGREE SIGN
        return toSymbol(apostrophe, 2);
    case '+': // U+002B PLUS SIGN
        return toSymbol(plusSign);
    case ',': // U+002C COMMA
    case '.': // U+002E FULL STOP
        return toSymbol(apostrophe);
    case '-': // U+002D HYPHEN-MINUS
        return toSymbol(hyphenMinus, 1);
    case '^': // U+005E CIRCUMFLEX ACCENT
        return toSymbol(circumflexAccent, 1);
    case '_': // U+005F LOW LINE
        return toSymbol(hyphenMinus);
    default:
        return {};
    }
    // NOLINTEND(bugprone-branch-clone)
}

#endif // FONT_MICRO
