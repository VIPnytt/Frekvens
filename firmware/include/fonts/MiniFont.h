#pragma once

#if FONT_MINI

#include "modules/FontModule.h"

#include <array>

class MiniFont final : public FontModule
{
private:
    // U+002B + PLUS SIGN
    static constexpr std::array<uint8_t, 3U> plusSign{
        0b010U,
        0b111U,
        0b010U,
    };

    // U+002C , COMMA
    static constexpr std::array<uint8_t, 2U> comma{
        0b01U,
        0b10U,
    };

    // U+002D - HYPHEN-MINUS
    // U+005F _ LOW LINE
    static constexpr std::array<uint8_t, 1U> hyphenMinus{0b111U};

    // U+002E . FULL STOP
    static constexpr std::array<uint8_t, 1U> fullStop{0b1U};

    static constexpr std::array<std::array<uint8_t, 5U>, 10U> digitZero_digitNine{{
        // U+0030 0 DIGIT ZERO
        {
            0b111U,
            0b101U,
            0b101U,
            0b101U,
            0b111U,
        },
        // U+0031 1 DIGIT ONE
        {
            0b010U,
            0b110U,
            0b010U,
            0b010U,
            0b111U,
        },
        // U+0032 2 DIGIT TWO
        {
            0b111U,
            0b001U,
            0b111U,
            0b100U,
            0b111U,
        },
        // U+0033 3 DIGIT THREE
        {
            0b111U,
            0b001U,
            0b111U,
            0b001U,
            0b111U,
        },
        // U+0034 4 DIGIT FOUR
        {
            0b101U,
            0b101U,
            0b111U,
            0b001U,
            0b001U,
        },
        // U+0035 5 DIGIT FIVE
        {
            0b111U,
            0b100U,
            0b111U,
            0b001U,
            0b111U,
        },
        // U+0036 6 DIGIT SIX
        {
            0b111U,
            0b100U,
            0b111U,
            0b101U,
            0b111U,
        },
        // U+0037 7 DIGIT SEVEN
        {
            0b111U,
            0b001U,
            0b001U,
            0b001U,
            0b001U,
        },
        // U+0038 8 DIGIT EIGHT
        {
            0b111U,
            0b101U,
            0b111U,
            0b101U,
            0b111U,
        },
        // U+0039 9 DIGIT NINE
        {
            0b111U,
            0b101U,
            0b111U,
            0b001U,
            0b111U,
        },
    }};

    // U+003A : COLON
    static constexpr std::array<uint8_t, 3U> colon{
        0b1U,
        0b0U,
        0b1U,
    };

    // U+003D = EQUALS SIGN
    static constexpr std::array<uint8_t, 3U> equalsSign{
        0b111U,
        0b000U,
        0b111U,
    };

    static constexpr std::array<std::array<uint8_t, 5U>, 29U> latinCapitalLetterA_rightSquareBracket{{
        // U+0041 A LATIN CAPITAL LETTER A
        {
            0b010U,
            0b101U,
            0b111U,
            0b101U,
            0b101U,
        },
        // U+0042 B LATIN CAPITAL LETTER B
        {
            0b110U,
            0b101U,
            0b110U,
            0b101U,
            0b110U,
        },
        // U+0043 C LATIN CAPITAL LETTER C
        {
            0b111U,
            0b100U,
            0b100U,
            0b100U,
            0b111U,
        },
        // U+0044 D LATIN CAPITAL LETTER D
        {
            0b110U,
            0b101U,
            0b101U,
            0b101U,
            0b110U,
        },
        // U+0045 E LATIN CAPITAL LETTER E
        {
            0b111U,
            0b100U,
            0b110U,
            0b100U,
            0b111U,
        },
        // U+0046 F LATIN CAPITAL LETTER F
        {
            0b111U,
            0b100U,
            0b110U,
            0b100U,
            0b100U,
        },
        // U+0047 G LATIN CAPITAL LETTER G
        {
            0b011U,
            0b100U,
            0b111U,
            0b101U,
            0b110U,
        },
        // U+0048 H LATIN CAPITAL LETTER H
        {
            0b101U,
            0b101U,
            0b111U,
            0b101U,
            0b101U,
        },
        // U+0049 I LATIN CAPITAL LETTER I
        {
            0b1U,
            0b1U,
            0b1U,
            0b1U,
            0b1U,
        },
        // U+004A J LATIN CAPITAL LETTER J
        {
            0b001U,
            0b001U,
            0b001U,
            0b101U,
            0b111U,
        },
        // U+004B K LATIN CAPITAL LETTER K
        {
            0b101U,
            0b101U,
            0b110U,
            0b111U,
            0b101U,
        },
        // U+004C L LATIN CAPITAL LETTER L
        {
            0b100U,
            0b100U,
            0b100U,
            0b100U,
            0b111U,
        },
        // U+004D M LATIN CAPITAL LETTER M
        {
            0b10001U,
            0b11011U,
            0b10101U,
            0b10001U,
            0b10001U,
        },
        // U+004E N LATIN CAPITAL LETTER N
        {
            0b10001U,
            0b11001U,
            0b10101U,
            0b10011U,
            0b10001U,
        },
        // U+004F O LATIN CAPITAL LETTER O
        {
            0b010U,
            0b101U,
            0b101U,
            0b101U,
            0b010U,
        },
        // U+0050 P LATIN CAPITAL LETTER P
        {
            0b110U,
            0b101U,
            0b110U,
            0b100U,
            0b100U,
        },
        // U+0051 Q LATIN CAPITAL LETTER Q
        {
            0b010U,
            0b101U,
            0b101U,
            0b111U,
            0b011U,
        },
        // U+0052 R LATIN CAPITAL LETTER R
        {
            0b110U,
            0b101U,
            0b110U,
            0b110U,
            0b101U,
        },
        // U+0053 S LATIN CAPITAL LETTER S
        {
            0b111U,
            0b100U,
            0b111U,
            0b001U,
            0b111U,
        },
        // U+0054 T LATIN CAPITAL LETTER T
        {
            0b111U,
            0b010U,
            0b010U,
            0b010U,
            0b010U,
        },
        // U+0055 U LATIN CAPITAL LETTER U
        {
            0b101U,
            0b101U,
            0b101U,
            0b101U,
            0b111U,
        },
        // U+0056 V LATIN CAPITAL LETTER V
        {
            0b101U,
            0b101U,
            0b101U,
            0b101U,
            0b010U,
        },
        // U+0057 W LATIN CAPITAL LETTER W
        {
            0b10101U,
            0b10101U,
            0b10101U,
            0b10101U,
            0b01010U,
        },
        // U+0058 X LATIN CAPITAL LETTER X
        {
            0b101U,
            0b101U,
            0b010U,
            0b101U,
            0b101U,
        },
        // U+0059 Y LATIN CAPITAL LETTER Y
        {
            0b101U,
            0b101U,
            0b101U,
            0b010U,
            0b010U,
        },
        // U+005A Z LATIN CAPITAL LETTER Z
        {
            0b111U,
            0b001U,
            0b010U,
            0b100U,
            0b111U,
        },
        // U+005B [ LEFT SQUARE BRACKET
        {
            0b11U,
            0b10U,
            0b10U,
            0b10U,
            0b11U,
        },
        // U+005C REVERSE SOLIDUS
        {
            0b110U,
            0b010U,
            0b010U,
            0b010U,
            0b001U,
        },
        // U+005D ] RIGHT SQUARE BRACKET
        {
            0b11U,
            0b01U,
            0b01U,
            0b01U,
            0b11U,
        },
    }};

    // U+0061 a LATIN SMALL LETTER A
    static constexpr std::array<uint8_t, 5U> latinSmallLetterA{
        0b110U,
        0b001U,
        0b011U,
        0b101U,
        0b111U,
    };

    // U+0062 b LATIN SMALL LETTER B
    static constexpr std::array<uint8_t, 4U> latinSmallLetterB{
        0b100U,
        0b110U,
        0b101U,
        0b110U,
    };

    // U+0063 c LATIN SMALL LETTER C
    static constexpr std::array<uint8_t, 3U> latinSmallLetterC{
        0b011U,
        0b100U,
        0b011U,
    };

    // U+0064 d LATIN SMALL LETTER D
    static constexpr std::array<uint8_t, 4U> latinSmallLetterD{
        0b001U,
        0b011U,
        0b101U,
        0b011U,
    };

    static constexpr std::array<std::array<uint8_t, 5U>, 3U> latinSmallLetterE_latinSmallLetterG{{
        // U+0065 e LATIN SMALL LETTER E
        {
            0b010U,
            0b101U,
            0b111U,
            0b100U,
            0b011U,
        },
        // U+0066 f LATIN SMALL LETTER F
        {
            0b011U,
            0b010U,
            0b111U,
            0b010U,
            0b010U,
        },
        // U+0067 g LATIN SMALL LETTER G
        {
            0b011U,
            0b101U,
            0b111U,
            0b111U,
            0b010U,
        },
    }};

    static constexpr std::array<std::array<uint8_t, 4U>, 2U> latinSmallLetterH_latinSmallLetterI{{
        // U+0068 h LATIN SMALL LETTER H
        {
            0b100U,
            0b100U,
            0b110U,
            0b101U,
        },
        // U+0069 i LATIN SMALL LETTER I
        {
            0b1U,
            0b0U,
            0b1U,
            0b1U,
        },
    }};

    // U+006A j LATIN SMALL LETTER J
    static constexpr std::array<uint8_t, 5U> latinSmallLetterJ{
        0b01U,
        0b00U,
        0b01U,
        0b01U,
        0b10U,
    };

    static constexpr std::array<std::array<uint8_t, 4U>, 2U> latinSmallLetterK_latinSmallLetterL{{
        // U+006B k LATIN SMALL LETTER K
        {
            0b101U,
            0b110U,
            0b110U,
            0b101U,
        },
        // U+006C l LATIN SMALL LETTER L
        {
            0b1U,
            0b1U,
            0b1U,
            0b1U,
        },
    }};

    static constexpr std::array<std::array<uint8_t, 3U>, 2U> latinSmallLetterM_latinSmallLetterN{{
        // U+006D m LATIN SMALL LETTER M
        {
            0b01010U,
            0b10101U,
            0b10101U,
        },
        // U+006E n LATIN SMALL LETTER N
        {
            0b110U,
            0b101U,
            0b101U,
        },
    }};

    static constexpr std::array<std::array<uint8_t, 4U>, 4U> latinSmallLetterO_latinSmallLetterR{{
        // U+006F o LATIN SMALL LETTER O
        {
            0b010U,
            0b101U,
            0b101U,
            0b010U,
        },
        // U+0070 p LATIN SMALL LETTER P
        {
            0b110U,
            0b101U,
            0b110U,
            0b100U,
        },
        // U+0071 q LATIN SMALL LETTER Q
        {
            0b011U,
            0b101U,
            0b011U,
            0b011U,
        },
        // U+0072 r LATIN SMALL LETTER R
        {
            0b101U,
            0b110U,
            0b100U,
            0b100U,
        },
    }};

    // U+0073 s LATIN SMALL LETTER S
    static constexpr std::array<uint8_t, 5U> latinSmallLetterS{
        0b011U,
        0b100U,
        0b010U,
        0b001U,
        0b110U,
    };

    // U+0074 t LATIN SMALL LETTER T
    static constexpr std::array<uint8_t, 4U> latinSmallLetterT{
        0b010U,
        0b111U,
        0b010U,
        0b011U,
    };

    static constexpr std::array<std::array<uint8_t, 3U>, 4U> latinSmallLetterU_latinSmallLetterX{{
        // U+0075 u LATIN SMALL LETTER U
        {
            0b101U,
            0b101U,
            0b011U,
        },
        // U+0076 v LATIN SMALL LETTER V
        {
            0b101U,
            0b101U,
            0b010U,
        },
        // U+0077 w LATIN SMALL LETTER W
        {
            0b10101U,
            0b10101U,
            0b01110U,
        },
        // U+0078 x LATIN SMALL LETTER X
        {
            0b101U,
            0b111U,
            0b101U,
        },
    }};

    // U+0079 y LATIN SMALL LETTER Y
    static constexpr std::array<uint8_t, 4U> latinSmallLetterY{
        0b101U,
        0b101U,
        0b010U,
        0b010U,
    };

    // U+007A z LATIN SMALL LETTER Z
    static constexpr std::array<uint8_t, 3U> latinSmallLetterZ{
        0b111U,
        0b010U,
        0b111U,
    };

    // U+007C | VERTICAL LINE
    static constexpr std::array<uint8_t, 5U> verticalLine{
        0b1U,
        0b1U,
        0b1U,
        0b1U,
        0b1U,
    };

    // U+00B0 ° DEGREE SIGN
    static constexpr std::array<uint8_t, 2U> degreeSign{
        0b11U,
        0b11U,
    };

    // U+03C0 π GREEK SMALL LETTER PI
    static constexpr std::array<uint8_t, 4U> greekSmallLetterPi{
        0b11111U,
        0b01010U,
        0b01010U,
        0b01010U,
    };

public:
    static constexpr std::string_view name{"Mini"};

    explicit MiniFont() : FontModule(name) {};

    [[nodiscard]] FontModule::Symbol getChar(char32_t character) const override;
};

#endif // FONT_MINI
