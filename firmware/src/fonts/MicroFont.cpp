#if FONT_MICRO

#include "fonts/MicroFont.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS >= 3U, __STRING(FONT_MICRO) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 3U, __STRING(FONT_MICRO) " is not compatible with this device's display size.");

FontModule::Symbol MicroFont::getChar(char32_t character) const
{
    if (character == '(' || character == ')')
    {
        return toSymbol(leftParenthesis_rightParenthesis[character - '(']);
    }
    if (character >= '/' && character <= '>')
    {
        return toSymbol(solidus_greaterThanSign[character - '/']);
    }
    if (character >= 'A' && character <= 'Z')
    {
        return toSymbol(latinLetterA_latinLetterZ[character - 'A']);
    }
    if (character >= 'X' && character <= ']')
    {
        return toSymbol(leftSquareBracket_rightSquareBracket[character - 'X']);
    }
    if (character >= 'a' && character <= 'z')
    {
        return toSymbol(latinLetterA_latinLetterZ[character - 'a']);
    }
    if (character >= '{' && character <= '}')
    {
        return toSymbol(leftCurlyBracket_rightCurlyBracket[character - '{']);
    }
    // NOLINTBEGIN(bugprone-branch-clone)
    switch (character)
    {
    case ' ': // SPACE
        return whitespace(3U);
    case '"': // QUOTATION MARK
        return toSymbol(quotationMark, 2);
    case '\'': // APOSTROPHE
    case '*':  // ASTERISK
    case '`':  // GRAVE ACCENT
    case U'°': // DEGREE SIGN
        return toSymbol(apostrophe, 2);
    case '+': // PLUS SIGN
        return toSymbol(plusSign);
    case ',': // COMMA
    case '.': // FULL STOP
        return toSymbol(apostrophe);
    case '-': // HYPHEN-MINUS
        return toSymbol(hyphenMinus, 1);
    case '^': // CIRCUMFLEX ACCENT
        return toSymbol(circumflexAccent, 1);
    case '_': // LOW LINE
        return toSymbol(hyphenMinus);
    }
    // NOLINTEND(bugprone-branch-clone)
    return {};
}

#endif // FONT_MICRO
