#if FONT_BRAILLE

#include "fonts/BrailleFont.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)

#include <vector>

static_assert(GRID_COLUMNS >= 2U, __STRING(FONT_BRAILLE) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 3U, __STRING(FONT_BRAILLE) " is not compatible with this device's display size.");

FontModule::Symbol BrailleFont::toSymbol(uint8_t bits) const
{
    const uint8_t row1{static_cast<uint8_t>(bits & 0b11U)};
    const uint8_t row2{static_cast<uint8_t>((bits >> 2U) & 0b11U)};
    const uint8_t row3{static_cast<uint8_t>((bits >> 4U) & 0b11U)};
    int8_t size{0};
    if (row3)
    {
        size = 3;
    }
    else if (row2)
    {
        size = 2;
    }
    else if (row1)
    {
        size = 1;
    }
    std::vector<uint8_t> bitmap(size);
    if (size)
    {
        bitmap[0U] = row1;
        if (size >= 2)
        {
            bitmap[1U] = row2;
            if (size == 3)
            {
                bitmap[2U] = row3;
            }
        }
    }
    return {bitmap,
            static_cast<uint8_t>(((row1 | row2 | row3) & (0b1U << 1U)) != 0U ? 0U : 1U),
            static_cast<int8_t>(3 - size)};
}

FontModule::Symbol BrailleFont::getChar(char32_t character) const
{
    if (character >= '1' && character <= '9')
    {
        // U+0031-U+0039
        return toSymbol(latinLetterA_latinLetterZ[character - '1']);
    }
    if (character >= 'A' && character <= 'Z')
    {
        // U+0041-U+005A
        return toSymbol(latinLetterA_latinLetterZ[character - 'A']);
    }
    if (character >= 'a' && character <= 'z')
    {
        // U+0061-U+007A
        return toSymbol(latinLetterA_latinLetterZ[character - 'a']);
    }
    // NOLINTBEGIN(bugprone-branch-clone)
    switch (character)
    {
    case ' ': // U+0020 SPACE
        return whitespace(2U);
    case '!': // U+0021 EXCLAMATION MARK
        return toSymbol(0b00'11'10U);
    case '\'': // U+0027 APOSTROPHE
        return toSymbol(0b00'00'10U);
    case '(': // U+0028 LEFT PARENTHESIS
    case ')': // U+0029 RIGHT PARENTHESIS
    case '[': // U+005B LEFT SQUARE BRACKET
    case ']': // U+005D RIGHT SQUARE BRACKET
    case '{': // U+007B LEFT CURLY BRACKET
    case '}': // U+007D RIGHT CURLY BRACKET
        return toSymbol(0b00'11'11U);
    case ',': // U+002C COMMA
        return toSymbol(0b00'10'00U);
    case '-': // U+002D HYPHEN-MINUS
        return toSymbol(0b00'00'11U);
    case '.': // U+002E FULL STOP
        return toSymbol(0b00'11'01U);
    case '/': // U+002F SOLIDUS
        return toSymbol(0b01'00'10U);
    case '0': // U+0030 DIGIT ZERO
        return toSymbol(latinLetterA_latinLetterZ[9U]);
    case ':': // U+003A COLON
        return toSymbol(0b00'11'00U);
    case ';': // U+003B SEMICOLON
        return toSymbol(0b00'10'10U);
    case '?': // U+003F QUESTION MARK
        return toSymbol(0b00'10'11U);
    default:
        return {};
    }
    // NOLINTEND(bugprone-branch-clone)
}

#endif // FONT_BRAILLE
