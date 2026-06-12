#pragma once

#if FONT_SMALL

#include "modules/FontModule.h"

#include <array>

class SmallFont final : public FontModule
{
private:
    // U+0021 ! EXCLAMATION MARK
    static constexpr std::array<uint8_t, 7U> exclamationMark{
        0b1U,
        0b1U,
        0b1U,
        0b1U,
        0b1U,
        0b0U,
        0b1U,
    };

    // U+0022 " QUOTATION MARK
    static constexpr std::array<uint8_t, 3U> quotationMark{
        0b101U,
        0b101U,
        0b101U,
    };

    static constexpr std::array<std::array<uint8_t, 7U>, 4U> numberSign_ampersand{{
        // U+0023 # NUMBER SIGN
        {
            0b01010U,
            0b01010U,
            0b11111U,
            0b01010U,
            0b11111U,
            0b01010U,
            0b01010U,
        },
        // U+0024 $ DOLLAR SIGN
        {
            0b00100U,
            0b01111U,
            0b10100U,
            0b01110U,
            0b00101U,
            0b11110U,
            0b00100U,
        },
        // U+0025 % PERCENT SIGN
        {
            0b11000U,
            0b11001U,
            0b00010U,
            0b00100U,
            0b01000U,
            0b10011U,
            0b00011U,
        },
        // U+0026 & AMPERSAND
        {
            0b0110000U,
            0b1001000U,
            0b1010000U,
            0b0100000U,
            0b1010101U,
            0b1001000U,
            0b0110100U,
        },
    }};

    // U+0027 ' APOSTROPHE
    static constexpr std::array<uint8_t, 3U> apostrophe{
        0b11U,
        0b01U,
        0b10U,
    };

    static constexpr std::array<std::array<uint8_t, 7U>, 2U> leftParenthesis_rightParenthesis{{
        // U+0028 ( LEFT PARENTHESIS
        {
            0b001U,
            0b010U,
            0b100U,
            0b100U,
            0b100U,
            0b010U,
            0b001U,
        },
        // U+0029 ) RIGHT PARENTHESIS
        {
            0b100U,
            0b010U,
            0b001U,
            0b001U,
            0b001U,
            0b010U,
            0b100U,
        },
    }};

    // U+002A * ASTERISK
    static constexpr std::array<uint8_t, 2U> asterisk{
        0b11U,
        0b11U,
    };

    // U+002B + PLUS SIGN
    static constexpr std::array<uint8_t, 5U> plusSign{
        0b00100U,
        0b00100U,
        0b11111U,
        0b00100U,
        0b00100U,
    };

    // U+002C , COMMA
    static constexpr std::array<uint8_t, 2U> comma{
        0b01U,
        0b10U,
    };

    // U+002D - HYPHEN-MINUS
    // U+005F _ LOW LINE
    static constexpr std::array<uint8_t, 1U> hyphenMinus{0b11111U};

    // U+002E . FULL STOP
    static constexpr std::array<uint8_t, 2U> fullStop{
        0b11U,
        0b11U,
    };

    // U+002F / SOLIDUS
    static constexpr std::array<uint8_t, 5U> solidus{
        0b00001U,
        0b00010U,
        0b00100U,
        0b01000U,
        0b10000U,
    };

    static constexpr std::array<std::array<uint8_t, 7U>, 10U> digitZero_digitNine{{
        // U+0030 0 DIGIT ZERO
        {
            0b0110U,
            0b1111U,
            0b1001U,
            0b1001U,
            0b1001U,
            0b1111U,
            0b0110U,
        },
        // U+0031 1 DIGIT ONE
        {
            0b011U,
            0b111U,
            0b011U,
            0b011U,
            0b011U,
            0b011U,
            0b011U,
        },
        // U+0032 2 DIGIT TWO
        {
            0b0110U,
            0b1111U,
            0b0001U,
            0b0100U,
            0b1000U,
            0b1111U,
            0b1111U,
        },
        // U+0033 3 DIGIT THREE
        {
            0b0110U,
            0b1111U,
            0b0001U,
            0b0111U,
            0b0001U,
            0b1111U,
            0b0110U,
        },
        // U+0034 4 DIGIT FOUR
        {
            0b1001U,
            0b1001U,
            0b1111U,
            0b0111U,
            0b0001U,
            0b0001U,
            0b0001U,
        },
        // U+0035 5 DIGIT FIVE
        {
            0b1111U,
            0b1111U,
            0b1000U,
            0b1110U,
            0b0001U,
            0b1111U,
            0b1110U,
        },
        // U+0036 6 DIGIT SIX
        {
            0b0110U,
            0b1111U,
            0b1000U,
            0b1110U,
            0b1001U,
            0b1111U,
            0b0110U,
        },
        // U+0037 7 DIGIT SEVEN
        {
            0b1111U,
            0b1111U,
            0b0010U,
            0b0100U,
            0b0100U,
            0b0100U,
            0b0100U,
        },
        // U+0038 8 DIGIT EIGHT
        {
            0b0110U,
            0b1111U,
            0b1001U,
            0b0110U,
            0b1001U,
            0b1111U,
            0b0110U,
        },
        // U+0039 9 DIGIT NINE
        {
            0b0110U,
            0b1111U,
            0b1001U,
            0b0111U,
            0b0001U,
            0b1111U,
            0b0110U,
        },
    }};

    // U+003A : COLON
    static constexpr std::array<uint8_t, 5U> colon{
        0b11U,
        0b11U,
        0b00U,
        0b11U,
        0b11U,
    };

    // U+003B ; SEMICOLON
    static constexpr std::array<uint8_t, 6U> semicolon{
        0b11U,
        0b11U,
        0b00U,
        0b11U,
        0b01U,
        0b10U,
    };

    // U+003C < LESS-THAN SIGN
    static constexpr std::array<uint8_t, 7U> lessThanSign{
        0b0001U,
        0b0010U,
        0b0100U,
        0b1000U,
        0b0100U,
        0b0010U,
        0b0001U,
    };

    // U+003D = EQUALS SIGN
    static constexpr std::array<uint8_t, 3U> equalsSign{
        0b11111U,
        0b00000U,
        0b11111U,
    };

    static constexpr std::array<std::array<uint8_t, 7U>, 30U> greaterThanSign_leftSquareBracket{{
        // U+003E > GREATER-THAN SIGN
        {
            0b1000U,
            0b0100U,
            0b0010U,
            0b0001U,
            0b0010U,
            0b0100U,
            0b1000U,
        },
        // U+003F ? QUESTION MARK
        {
            0b01110U,
            0b10001U,
            0b00001U,
            0b00010U,
            0b00100U,
            0b00000U,
            0b00100U,
        },
        // U+0040 @ COMMERCIAL AT
        {
            0b01110U,
            0b10001U,
            0b00001U,
            0b01101U,
            0b10101U,
            0b10101U,
            0b01110U,
        },
        // U+0041 A LATIN CAPITAL LETTER A
        {
            0b01110U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b11111U,
            0b10001U,
            0b10001U,
        },
        // U+0042 B LATIN CAPITAL LETTER B
        {
            0b11110U,
            0b10001U,
            0b10001U,
            0b11110U,
            0b10001U,
            0b10001U,
            0b11110U,
        },
        // U+0043 C LATIN CAPITAL LETTER C
        {
            0b01110U,
            0b10001U,
            0b10000U,
            0b10000U,
            0b10000U,
            0b10001U,
            0b01110U,
        },
        // U+0044 D LATIN CAPITAL LETTER D
        {
            0b11100U,
            0b10010U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10010U,
            0b11100U,
        },
        // U+0045 E LATIN CAPITAL LETTER E
        {
            0b11111U,
            0b10000U,
            0b10000U,
            0b11110U,
            0b10000U,
            0b10000U,
            0b11111U,
        },
        // U+0046 F LATIN CAPITAL LETTER F
        {
            0b11111U,
            0b10000U,
            0b10000U,
            0b11100U,
            0b10000U,
            0b10000U,
            0b10000U,
        },
        // U+0047 G LATIN CAPITAL LETTER G
        {
            0b01110U,
            0b10001U,
            0b10000U,
            0b10000U,
            0b10011U,
            0b10001U,
            0b01110U,
        },
        // U+0048 H LATIN CAPITAL LETTER H
        {
            0b10001U,
            0b10001U,
            0b10001U,
            0b11111U,
            0b10001U,
            0b10001U,
            0b10001U,
        },
        // U+0049 I LATIN CAPITAL LETTER I
        {
            0b111U,
            0b010U,
            0b010U,
            0b010U,
            0b010U,
            0b010U,
            0b111U,
        },
        // U+004A J LATIN CAPITAL LETTER J
        {
            0b00111U,
            0b00010U,
            0b00010U,
            0b00010U,
            0b00010U,
            0b10010U,
            0b01100U,
        },
        // U+004B K LATIN CAPITAL LETTER K
        {
            0b10001U,
            0b10010U,
            0b10100U,
            0b11000U,
            0b10100U,
            0b10010U,
            0b10001U,
        },
        // U+004C L LATIN CAPITAL LETTER L
        {
            0b10000U,
            0b10000U,
            0b10000U,
            0b10000U,
            0b10000U,
            0b10000U,
            0b11111U,
        },
        // U+004D M LATIN CAPITAL LETTER M
        {
            0b10001U,
            0b11011U,
            0b10101U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
        },
        // U+004E N LATIN CAPITAL LETTER N
        {
            0b10001U,
            0b10001U,
            0b11001U,
            0b10101U,
            0b10011U,
            0b10001U,
            0b10001U,
        },
        // U+004F O LATIN CAPITAL LETTER O
        {
            0b01110U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b01110U,
        },
        // U+0050 P LATIN CAPITAL LETTER P
        {
            0b11110U,
            0b10001U,
            0b10001U,
            0b11110U,
            0b10000U,
            0b10000U,
            0b10000U,
        },
        // U+0051 Q LATIN CAPITAL LETTER Q
        {
            0b01110U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10101U,
            0b10010U,
            0b01101U,
        },
        // U+0052 R LATIN CAPITAL LETTER R
        {
            0b11110U,
            0b10001U,
            0b10001U,
            0b11110U,
            0b10100U,
            0b10010U,
            0b10001U,
        },
        // U+0053 S LATIN CAPITAL LETTER S
        {
            0b01111U,
            0b10000U,
            0b10000U,
            0b01110U,
            0b00001U,
            0b00001U,
            0b11110U,
        },
        // U+0054 T LATIN CAPITAL LETTER T
        {
            0b11111U,
            0b00100U,
            0b00100U,
            0b00100U,
            0b00100U,
            0b00100U,
            0b00100U,
        },
        // U+0055 U LATIN CAPITAL LETTER U
        {
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b01110U,
        },
        // U+0056 V LATIN CAPITAL LETTER V
        {
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b01010U,
            0b00100U,
        },
        // U+0057 W LATIN CAPITAL LETTER W
        {
            0b10001U,
            0b10001U,
            0b10001U,
            0b10101U,
            0b10101U,
            0b11011U,
            0b10001U,
        },
        // U+0058 X LATIN CAPITAL LETTER X
        {
            0b10001U,
            0b10001U,
            0b01010U,
            0b00100U,
            0b01010U,
            0b10001U,
            0b10001U,
        },
        // U+0059 Y LATIN CAPITAL LETTER Y
        {
            0b10001U,
            0b10001U,
            0b01010U,
            0b00100U,
            0b00100U,
            0b00100U,
            0b00100U,
        },
        // U+005A Z LATIN CAPITAL LETTER Z
        {
            0b11111U,
            0b00001U,
            0b00010U,
            0b00100U,
            0b01000U,
            0b10000U,
            0b11111U,
        },
        // U+005B [ LEFT SQUARE BRACKET
        {
            0b111U,
            0b100U,
            0b100U,
            0b100U,
            0b100U,
            0b100U,
            0b111U,
        },
    }};

    // U+005C REVERSE SOLIDUS
    static constexpr std::array<uint8_t, 5U> reverseSolidus{
        0b10000U,
        0b01000U,
        0b00100U,
        0b00010U,
        0b00001U,
    };

    // U+005D ] RIGHT SQUARE BRACKET
    static constexpr std::array<uint8_t, 7U> rightSquareBracket{
        0b111U,
        0b001U,
        0b001U,
        0b001U,
        0b001U,
        0b001U,
        0b111U,
    };

    // U+005E ^ CIRCUMFLEX ACCENT
    static constexpr std::array<uint8_t, 3U> circumflexAccent{
        0b00100U,
        0b01010U,
        0b10001U,
    };

    // U+0060 ` GRAVE ACCENT
    static constexpr std::array<uint8_t, 3U> graveAccent{
        0b100U,
        0b010U,
        0b001U,
    };

    // U+0061 a LATIN SMALL LETTER A
    static constexpr std::array<uint8_t, 5U> latinSmallLetterA{
        0b01110U,
        0b00001U,
        0b01111U,
        0b10001U,
        0b01111U,
    };

    // U+0062 b LATIN SMALL LETTER B
    static constexpr std::array<uint8_t, 7U> latinSmallLetterB{
        0b10000U,
        0b10000U,
        0b10110U,
        0b11001U,
        0b10001U,
        0b10001U,
        0b11110U,
    };

    // U+0063 c LATIN SMALL LETTER C
    static constexpr std::array<uint8_t, 5U> latinSmallLetterC{
        0b01110U,
        0b10000U,
        0b10000U,
        0b10001U,
        0b01110U,
    };

    // U+0064 d LATIN SMALL LETTER D
    static constexpr std::array<uint8_t, 7U> latinSmallLetterD{
        0b00001U,
        0b00001U,
        0b01101U,
        0b10011U,
        0b10001U,
        0b10001U,
        0b01111U,
    };

    // U+0065 e LATIN SMALL LETTER E
    static constexpr std::array<uint8_t, 5U> latinSmallLetterE{
        0b01110U,
        0b10001U,
        0b11111U,
        0b10000U,
        0b01110U,
    };

    // U+0066 f LATIN SMALL LETTER F
    static constexpr std::array<uint8_t, 7U> latinSmallLetterF{
        0b00110U,
        0b01001U,
        0b01000U,
        0b11100U,
        0b01000U,
        0b01000U,
        0b01000U,
    };

    // U+0067 g LATIN SMALL LETTER G
    static constexpr std::array<uint8_t, 5U> latinSmallLetterG{
        0b01111U,
        0b10001U,
        0b01111U,
        0b00001U,
        0b00110U,
    };

    static constexpr std::array<std::array<uint8_t, 7U>, 5U> latinSmallLetterH_latinSmallLetterL{{
        // U+0068 h LATIN SMALL LETTER H
        {
            0b10000U,
            0b10000U,
            0b10110U,
            0b11001U,
            0b10001U,
            0b10001U,
            0b10001U,
        },
        // U+0069 i LATIN SMALL LETTER I
        {
            0b010U,
            0b000U,
            0b110U,
            0b010U,
            0b010U,
            0b010U,
            0b111U,
        },
        // U+006A j LATIN SMALL LETTER J
        {
            0b0001U,
            0b0000U,
            0b0011U,
            0b0001U,
            0b0001U,
            0b1001U,
            0b0110U,
        },
        // U+006B k LATIN SMALL LETTER K
        {
            0b10000U,
            0b10000U,
            0b10001U,
            0b10010U,
            0b11100U,
            0b10010U,
            0b10001U,
        },
        // U+006C l LATIN SMALL LETTER L
        {
            0b110U,
            0b010U,
            0b010U,
            0b010U,
            0b010U,
            0b010U,
            0b111U,
        },
    }};

    static constexpr std::array<std::array<uint8_t, 5U>, 7U> latinSmallLetterM_latinSmallLetterS{{
        // U+006D m LATIN SMALL LETTER M
        {
            0b11010U,
            0b10101U,
            0b10101U,
            0b10001U,
            0b10001U,
        },
        // U+006E n LATIN SMALL LETTER N
        {
            0b10110U,
            0b11001U,
            0b10001U,
            0b10001U,
            0b10001U,
        },
        // U+006F o LATIN SMALL LETTER O
        {
            0b01110U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b01110U,
        },
        // U+0070 p LATIN SMALL LETTER P
        {
            0b11110U,
            0b10001U,
            0b11110U,
            0b10000U,
            0b10000U,
        },
        // U+0071 q LATIN SMALL LETTER Q
        {
            0b01101U,
            0b10011U,
            0b01111U,
            0b00001U,
            0b00001U,
        },
        // U+0072 r LATIN SMALL LETTER R
        {
            0b10110U,
            0b11001U,
            0b10000U,
            0b10000U,
            0b10000U,
        },
        // U+0073 s LATIN SMALL LETTER S
        {
            0b01110U,
            0b10000U,
            0b01110U,
            0b00001U,
            0b11110U,
        },
    }};

    // U+0074 t LATIN SMALL LETTER T
    static constexpr std::array<uint8_t, 6U> latinSmallLetterT{
        0b01000U,
        0b01000U,
        0b11100U,
        0b01000U,
        0b01001U,
        0b00110U,
    };

    static constexpr std::array<std::array<uint8_t, 5U>, 6U> latinSmallLetterU_latinSmallLetterZ{{
        // U+0075 u LATIN SMALL LETTER U
        {
            0b10001U,
            0b10001U,
            0b10001U,
            0b10011U,
            0b01101U,
        },
        // U+0076 v LATIN SMALL LETTER V
        {
            0b10001U,
            0b10001U,
            0b10001U,
            0b01010U,
            0b00100U,
        },
        // U+0077 w LATIN SMALL LETTER W
        {
            0b10001U,
            0b10001U,
            0b10101U,
            0b10101U,
            0b01010U,
        },
        // U+0078 x LATIN SMALL LETTER X
        {
            0b10001U,
            0b01010U,
            0b00100U,
            0b01010U,
            0b10001U,
        },
        // U+0079 y LATIN SMALL LETTER Y
        {
            0b10001U,
            0b10001U,
            0b01111U,
            0b00001U,
            0b01110U,
        },
        // U+007A z LATIN SMALL LETTER Z
        {
            0b11111U,
            0b00010U,
            0b00100U,
            0b01000U,
            0b11111U,
        },
    }};

    static constexpr std::array<std::array<uint8_t, 7U>, 3U> leftCurlyBracket_rightCurlyBracket{{
        // U+007B { LEFT CURLY BRACKET
        {
            0b001U,
            0b010U,
            0b010U,
            0b100U,
            0b010U,
            0b010U,
            0b001U,
        },
        // U+007C | VERTICAL LINE
        {
            0b1U,
            0b1U,
            0b1U,
            0b1U,
            0b1U,
            0b1U,
            0b1U,
        },
        // U+007D } RIGHT CURLY BRACKET
        {
            0b100U,
            0b010U,
            0b010U,
            0b001U,
            0b010U,
            0b010U,
            0b100U,
        },
    }};

    // U+00B0 ° DEGREE SIGN
    static constexpr std::array<uint8_t, 2U> degreeSign{
        0b11U,
        0b11U,
    };

    // U+00C4 Ä LATIN CAPITAL LETTER A WITH DIAERESIS
    static constexpr std::array<uint8_t, 7U> latinCapitalLetterAWithDiaeresis{
        0b10001U,
        0b01110U,
        0b10001U,
        0b10001U,
        0b11111U,
        0b10001U,
        0b10001U,
    };

    // U+00C5 Å LATIN CAPITAL LETTER A WITH RING ABOVE
    static constexpr std::array<uint8_t, 7U> latinCapitalLetterAWithRingAbove{
        0b01110U,
        0b01110U,
        0b11111U,
        0b10001U,
        0b11111U,
        0b10001U,
        0b10001U,
    };

    // U+00C6 Æ LATIN CAPITAL LETTER AE
    static constexpr std::array<uint8_t, 5U> latinCapitalLetterAe{
        0b011111U,
        0b100100U,
        0b111110U,
        0b100100U,
        0b100111U,
    };

    // U+00D6 Ö LATIN CAPITAL LETTER O WITH DIAERESIS
    static constexpr std::array<uint8_t, 7U> latinCapitalLetterOWithDiaeresis{
        0b10001U,
        0b00000U,
        0b11111U,
        0b10001U,
        0b10001U,
        0b10001U,
        0b01110U,
    };

    // U+00D8 Ø LATIN CAPITAL LETTER O WITH STROKE
    static constexpr std::array<uint8_t, 5U> latinCapitalLetterOWithStroke{
        0b01111U,
        0b10011U,
        0b10101U,
        0b11001U,
        0b11110U,
    };

    // U+00DF ß LATIN SMALL LETTER SHARP S
    static constexpr std::array<uint8_t, 7U> latinSmallLetterSharpS{
        0b01110U,
        0b10001U,
        0b10001U,
        0b11111U,
        0b10001U,
        0b10001U,
        0b10111U,
    };

    // U+00E4 ä LATIN SMALL LETTER A WITH DIAERESIS
    static constexpr std::array<uint8_t, 6U> latinSmallLetterAWithDiaeresis{
        0b10001U,
        0b01110U,
        0b00001U,
        0b01111U,
        0b10001U,
        0b01111U,
    };

    // U+00E5 å LATIN SMALL LETTER A WITH RING ABOVE
    static constexpr std::array<uint8_t, 7U> latinSmallLetterAWithRingAbove{
        0b01110U,
        0b01110U,
        0b01110U,
        0b00001U,
        0b01111U,
        0b10001U,
        0b01111U,
    };

    // U+00E6 æ LATIN SMALL LETTER AE
    static constexpr std::array<uint8_t, 5U> latinSmallLetterAe{
        0b1111110U,
        0b0001001U,
        0b1111111U,
        0b1001000U,
        0b1111111U,
    };

    // U+00F6 ö LATIN SMALL LETTER O WITH DIAERESIS
    static constexpr std::array<uint8_t, 6U> latinSmallLetterOWithDiaeresis{
        0b10001U,
        0b01110U,
        0b10001U,
        0b10001U,
        0b10001U,
        0b01110U,
    };

    // U+00F8 ø LATIN SMALL LETTER O WITH STROKE
    static constexpr std::array<uint8_t, 5U> latinSmallLetterOWithStroke{
        0b01110U,
        0b10011U,
        0b10101U,
        0b11001U,
        0b01110U,
    };

    // U+00FC ü LATIN SMALL LETTER U WITH DIAERESIS
    static constexpr std::array<uint8_t, 6U> latinSmallLetterUWithDiaeresis{
        0b10001U,
        0b00000U,
        0b10001U,
        0b10001U,
        0b10011U,
        0b01101U,
    };

    // U+03C0 π GREEK SMALL LETTER PI
    static constexpr std::array<uint8_t, 5U> greekSmallLetterPi{
        0b11111U,
        0b01010U,
        0b01010U,
        0b01010U,
        0b01010U,
    };

public:
    static constexpr std::string_view name{"Small"};

    explicit SmallFont() : FontModule(name) {};

    [[nodiscard]] FontModule::Symbol getChar(char32_t character) const override;
};

#endif // FONT_SMALL
