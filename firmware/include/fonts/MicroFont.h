#pragma once

#if FONT_MICRO

#include "modules/FontModule.h"

#include <array>

class MicroFont final : public FontModule
{
private:
    static constexpr std::array<uint8_t, 1U> quotationMark{0b11U}; // U+0022 " QUOTATION MARK

    // U+0027 ' APOSTROPHE
    // U+002A * ASTERISK
    // U+002C , COMMA
    // U+002E . FULL STOP
    // U+0060 ` GRAVE ACCENT
    // U+00B0 ° DEGREE SIGN
    static constexpr std::array<uint8_t, 1U> apostrophe{0b1U};

    static constexpr std::array<std::array<uint8_t, 3U>, 2U> leftParenthesis_rightParenthesis{{
        // U+0028 ( LEFT PARENTHESIS
        {
            0b01U,
            0b10U,
            0b01U,
        },
        // U+0029 ) RIGHT PARENTHESIS
        {
            0b10U,
            0b01U,
            0b10U,
        },
    }};

    // U+002B + PLUS SIGN
    static constexpr std::array<uint8_t, 3U> plusSign{
        0b010U,
        0b111U,
        0b010U,
    };

    // U+002D - HYPHEN-MINUS
    // U+005F _ LOW LINE
    static constexpr std::array<uint8_t, 1U> hyphenMinus{0b111U};

    static constexpr std::array<std::array<uint8_t, 3U>, 16U> solidus_greaterThanSign{{
        // U+002F / SOLIDUS
        {
            0b001U,
            0b010U,
            0b100U,
        },
        // U+0030 0 DIGIT ZERO
        {
            0b111U,
            0b101U,
            0b111U,
        },
        // U+0031 1 DIGIT ONE
        {
            0b11U,
            0b01U,
            0b01U,
        },
        // U+0032 2 DIGIT TWO
        {
            0b110U,
            0b010U,
            0b011U,
        },
        // U+0033 3 DIGIT THREE
        {
            0b111U,
            0b011U,
            0b111U,
        },
        // U+0034 4 DIGIT FOUR
        {
            0b101U,
            0b111U,
            0b001U,
        },
        // U+0035 5 DIGIT FIVE
        {
            0b011U,
            0b010U,
            0b110U,
        },
        // U+0036 6 DIGIT SIX
        {
            0b100U,
            0b111U,
            0b111U,
        },
        // U+0037 7 DIGIT SEVEN
        {
            0b111U,
            0b011U,
            0b001U,
        },
        // U+0038 8 DIGIT EIGHT
        {
            0b111U,
            0b111U,
            0b111U,
        },
        // U+0039 9 DIGIT NINE
        {
            0b111U,
            0b111U,
            0b001U,
        },
        // U+003A : COLON
        {
            0b1U,
            0b0U,
            0b1U,
        },
        // U+003B ; SEMICOLON
        {
            0b01U,
            0b00U,
            0b11U,
        },
        // U+003C < LESS-THAN SIGN
        {
            0b01U,
            0b10U,
            0b01U,
        },
        // U+003D = EQUALS SIGN
        {
            0b111U,
            0b000U,
            0b111U,
        },
        // U+003E > GREATER-THAN SIGN
        {
            0b10U,
            0b01U,
            0b10U,
        },
    }};

    static constexpr std::array<std::array<uint8_t, 3U>, 26U> latinLetterA_latinLetterZ{{
        // U+0041 A LATIN CAPITAL LETTER A
        // U+0061 a LATIN SMALL LETTER A
        {
            0b011U,
            0b111U,
            0b111U,
        },
        // U+0042 B LATIN CAPITAL LETTER B
        // U+0062 b LATIN SMALL LETTER B
        {
            0b100U,
            0b111U,
            0b111U,
        },
        // U+0043 C LATIN CAPITAL LETTER C
        // U+0063 c LATIN SMALL LETTER C
        {
            0b111U,
            0b100U,
            0b111U,
        },
        // U+0044 D LATIN CAPITAL LETTER D
        // U+0064 d LATIN SMALL LETTER D
        {
            0b001U,
            0b111U,
            0b111U,
        },
        // U+0045 E LATIN CAPITAL LETTER E
        // U+0065 e LATIN SMALL LETTER E
        {
            0b111U,
            0b110U,
            0b111U,
        },
        // U+0046 F LATIN CAPITAL LETTER F
        // U+0066 f LATIN SMALL LETTER F
        {
            0b111U,
            0b110U,
            0b100U,
        },
        // U+0047 G LATIN CAPITAL LETTER G
        // U+0067 g LATIN SMALL LETTER G
        {
            0b111U,
            0b111U,
            0b111U,
        },
        // U+0048 H LATIN CAPITAL LETTER H
        // U+0068 h LATIN SMALL LETTER H
        {
            0b101U,
            0b111U,
            0b101U,
        },
        // U+0049 I LATIN CAPITAL LETTER I
        // U+0069 i LATIN SMALL LETTER I
        {
            0b111U,
            0b010U,
            0b111U,
        },
        // U+004A J LATIN CAPITAL LETTER J
        // U+006A j LATIN SMALL LETTER J
        {
            0b111U,
            0b010U,
            0b110U,
        },
        // U+004B K LATIN CAPITAL LETTER K
        // U+006B k LATIN SMALL LETTER K
        {
            0b101U,
            0b110U,
            0b101U,
        },
        // U+004C L LATIN CAPITAL LETTER L
        // U+006C l LATIN SMALL LETTER L
        {
            0b100U,
            0b100U,
            0b111U,
        },
        // U+004D M LATIN CAPITAL LETTER M
        // U+006D m LATIN SMALL LETTER M
        {
            0b111U,
            0b111U,
            0b101U,
        },
        // U+004E N LATIN CAPITAL LETTER N
        // U+006E n LATIN SMALL LETTER N
        {
            0b111U,
            0b101U,
            0b101U,
        },
        // U+004F O LATIN CAPITAL LETTER O
        // U+006F o LATIN SMALL LETTER O
        {
            0b111U,
            0b101U,
            0b111U,
        },
        // U+0050 P LATIN CAPITAL LETTER P
        // U+0070 p LATIN SMALL LETTER P
        {
            0b111U,
            0b111U,
            0b100U,
        },
        // U+0051 Q LATIN CAPITAL LETTER Q
        // U+0071 q LATIN SMALL LETTER Q
        {
            0b111U,
            0b111U,
            0b001U,
        },
        // U+0052 R LATIN CAPITAL LETTER R
        // U+0072 r LATIN SMALL LETTER R
        {
            0b111U,
            0b110U,
            0b101U,
        },
        // U+0053 S LATIN CAPITAL LETTER S
        // U+0073 s LATIN SMALL LETTER S
        {
            0b011U,
            0b010U,
            0b110U,
        },
        // U+0054 T LATIN CAPITAL LETTER T
        // U+0074 t LATIN SMALL LETTER T
        {
            0b111U,
            0b010U,
            0b010U,
        },
        // U+0055 U LATIN CAPITAL LETTER U
        // U+0075 u LATIN SMALL LETTER U
        {
            0b101U,
            0b101U,
            0b111U,
        },
        // U+0056 V LATIN CAPITAL LETTER V
        // U+0076 v LATIN SMALL LETTER V
        {
            0b101U,
            0b101U,
            0b010U,
        },
        // U+0057 W LATIN CAPITAL LETTER W
        // U+0077 w LATIN SMALL LETTER W
        {
            0b101U,
            0b111U,
            0b111U,
        },
        // U+0058 X LATIN CAPITAL LETTER X
        // U+0078 x LATIN SMALL LETTER X
        {
            0b101U,
            0b010U,
            0b101U,
        },
        // U+0059 Y LATIN CAPITAL LETTER Y
        // U+0079 y LATIN SMALL LETTER Y
        {
            0b101U,
            0b111U,
            0b010U,
        },
        // U+005A Z LATIN CAPITAL LETTER Z
        // U+007A z LATIN SMALL LETTER Z
        {
            0b110U,
            0b010U,
            0b011U,
        },
    }};

    static constexpr std::array<std::array<uint8_t, 3U>, 3U> leftSquareBracket_rightSquareBracket{{
        // U+005B [ LEFT SQUARE BRACKET
        {
            0b11U,
            0b10U,
            0b11U,
        },
        // U+005C REVERSE SOLIDUS
        {
            0b100U,
            0b010U,
            0b001U,
        },
        // U+005D ] RIGHT SQUARE BRACKET
        {
            0b11U,
            0b01U,
            0b11U,
        },
    }};

    // U+005E ^ CIRCUMFLEX ACCENT
    static constexpr std::array<uint8_t, 2U> circumflexAccent{
        0b010U,
        0b101U,
    };

    static constexpr std::array<std::array<uint8_t, 3U>, 3U> leftCurlyBracket_rightCurlyBracket{{
        // U+007B { LEFT CURLY BRACKET
        {
            0b001U,
            0b110U,
            0b001U,
        },
        // U+007C | VERTICAL LINE
        {
            0b1U,
            0b1U,
            0b1U,
        },
        // U+007D } RIGHT CURLY BRACKET
        {
            0b100U,
            0b011U,
            0b100U,
        },
    }};

public:
    static constexpr std::string_view name{"Micro"};

    explicit MicroFont() : FontModule(name) {};

    [[nodiscard]] FontModule::Symbol getChar(char32_t character) const override;
};

#endif // FONT_MICRO
