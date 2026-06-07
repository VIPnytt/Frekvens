#pragma once

#if FONT_SMALL

#include "modules/FontModule.h"

#include <array>

class SmallFont final : public FontModule
{
private:
    // 0x21, ! EXCLAMATION MARK
    static constexpr std::array<uint8_t, 7U> exclamationMark{
        0b1U,
        0b1U,
        0b1U,
        0b1U,
        0b1U,
        0b0U,
        0b1U,
    };

    // 0x22, " QUOTATION MARK
    static constexpr std::array<uint8_t, 3U> quotationMark{
        0b101U,
        0b101U,
        0b101U,
    };

    static constexpr std::array<std::array<uint8_t, 7U>, 4U> numberSign_ampersand{{
        // 0x23, # NUMBER SIGN
        {
            0b01010U,
            0b01010U,
            0b11111U,
            0b01010U,
            0b11111U,
            0b01010U,
            0b01010U,
        },
        // 0x24, $ DOLLAR SIGN
        {
            0b00100U,
            0b01111U,
            0b10100U,
            0b01110U,
            0b00101U,
            0b11110U,
            0b00100U,
        },
        // 0x25, % PERCENT SIGN
        {
            0b11000U,
            0b11001U,
            0b00010U,
            0b00100U,
            0b01000U,
            0b10011U,
            0b00011U,
        },
        // 0x26, & AMPERSAND
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

    // 0x27, ' APOSTROPHE
    static constexpr std::array<uint8_t, 3U> apostrophe{
        0b11U,
        0b01U,
        0b10U,
    };

    static constexpr std::array<std::array<uint8_t, 7U>, 2U> leftParenthesis_rightParenthesis{{
        // 0x28, ( LEFT PARENTHESIS
        {
            0b001U,
            0b010U,
            0b100U,
            0b100U,
            0b100U,
            0b010U,
            0b001U,
        },
        // 0x29, ) RIGHT PARENTHESIS
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

    // 0x2A, * ASTERISK
    static constexpr std::array<uint8_t, 2U> asterisk{
        0b11U,
        0b11U,
    };

    // 0x2B, + PLUS SIGN
    static constexpr std::array<uint8_t, 5U> plusSign{
        0b00100U,
        0b00100U,
        0b11111U,
        0b00100U,
        0b00100U,
    };

    // 0x2C, , COMMA
    static constexpr std::array<uint8_t, 2U> comma{
        0b01U,
        0b10U,
    };

    // 0x2D, - HYPHEN-MINUS
    // 0x5F, _ LOW LINE
    static constexpr std::array<uint8_t, 1U> hyphenMinus{0b11111U};

    // 0x2E, . FULL STOP
    static constexpr std::array<uint8_t, 2U> fullStop{
        0b11U,
        0b11U,
    };

    // 0x2F, / SOLIDUS
    static constexpr std::array<uint8_t, 5U> solidus{
        0b00001U,
        0b00010U,
        0b00100U,
        0b01000U,
        0b10000U,
    };

    static constexpr std::array<std::array<uint8_t, 7U>, 10U> digitZero_digitNine{{
        // 0x30, 0 DIGIT ZERO
        {
            0b0110U,
            0b1111U,
            0b1001U,
            0b1001U,
            0b1001U,
            0b1111U,
            0b0110U,
        },
        // 0x31, 1 DIGIT ONE
        {
            0b011U,
            0b111U,
            0b011U,
            0b011U,
            0b011U,
            0b011U,
            0b011U,
        },
        // 0x32, 2 DIGIT TWO
        {
            0b0110U,
            0b1111U,
            0b0001U,
            0b0100U,
            0b1000U,
            0b1111U,
            0b1111U,
        },
        // 0x33, 3 DIGIT THREE
        {
            0b0110U,
            0b1111U,
            0b0001U,
            0b0111U,
            0b0001U,
            0b1111U,
            0b0110U,
        },
        // 0x34, 4 DIGIT FOUR
        {
            0b1001U,
            0b1001U,
            0b1111U,
            0b0111U,
            0b0001U,
            0b0001U,
            0b0001U,
        },
        // 0x35, 5 DIGIT FIVE
        {
            0b1111U,
            0b1111U,
            0b1000U,
            0b1110U,
            0b0001U,
            0b1111U,
            0b1110U,
        },
        // 0x36, 6 DIGIT SIX
        {
            0b0110U,
            0b1111U,
            0b1000U,
            0b1110U,
            0b1001U,
            0b1111U,
            0b0110U,
        },
        // 0x37, 7 DIGIT SEVEN
        {
            0b1111U,
            0b1111U,
            0b0010U,
            0b0100U,
            0b0100U,
            0b0100U,
            0b0100U,
        },
        // 0x38, 8 DIGIT EIGHT
        {
            0b0110U,
            0b1111U,
            0b1001U,
            0b0110U,
            0b1001U,
            0b1111U,
            0b0110U,
        },
        // 0x39, 9 DIGIT NINE
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

    // 0x3A, : COLON
    static constexpr std::array<uint8_t, 5U> colon{
        0b11U,
        0b11U,
        0b00U,
        0b11U,
        0b11U,
    };

    // 0x3B, ; SEMICOLON
    static constexpr std::array<uint8_t, 6U> semicolon{
        0b11U,
        0b11U,
        0b00U,
        0b11U,
        0b01U,
        0b10U,
    };

    // 0x3C, < LESS-THAN SIGN
    static constexpr std::array<uint8_t, 7U> lessThanSign{
        0b0001U,
        0b0010U,
        0b0100U,
        0b1000U,
        0b0100U,
        0b0010U,
        0b0001U,
    };

    // 0x3D, = EQUALS SIGN
    static constexpr std::array<uint8_t, 3U> equalsSign{
        0b11111U,
        0b00000U,
        0b11111U,
    };

    static constexpr std::array<std::array<uint8_t, 7U>, 30U> greaterThanSign_leftSquareBracket{{
        // 0x3E, > GREATER-THAN SIGN
        {
            0b1000U,
            0b0100U,
            0b0010U,
            0b0001U,
            0b0010U,
            0b0100U,
            0b1000U,
        },
        // 0x3F, ? QUESTION MARK
        {
            0b01110U,
            0b10001U,
            0b00001U,
            0b00010U,
            0b00100U,
            0b00000U,
            0b00100U,
        },
        // 0x40, @ COMMERCIAL AT
        {
            0b01110U,
            0b10001U,
            0b00001U,
            0b01101U,
            0b10101U,
            0b10101U,
            0b01110U,
        },
        // 0x41, A LATIN CAPITAL LETTER A
        {
            0b01110U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b11111U,
            0b10001U,
            0b10001U,
        },
        // 0x42, B LATIN CAPITAL LETTER B
        {
            0b11110U,
            0b10001U,
            0b10001U,
            0b11110U,
            0b10001U,
            0b10001U,
            0b11110U,
        },
        // 0x43, C LATIN CAPITAL LETTER C
        {
            0b01110U,
            0b10001U,
            0b10000U,
            0b10000U,
            0b10000U,
            0b10001U,
            0b01110U,
        },
        // 0x44, D LATIN CAPITAL LETTER D
        {
            0b11100U,
            0b10010U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10010U,
            0b11100U,
        },
        // 0x45, E LATIN CAPITAL LETTER E
        {
            0b11111U,
            0b10000U,
            0b10000U,
            0b11110U,
            0b10000U,
            0b10000U,
            0b11111U,
        },
        // 0x46, F LATIN CAPITAL LETTER F
        {
            0b11111U,
            0b10000U,
            0b10000U,
            0b11100U,
            0b10000U,
            0b10000U,
            0b10000U,
        },
        // 0x47, G LATIN CAPITAL LETTER G
        {
            0b01110U,
            0b10001U,
            0b10000U,
            0b10000U,
            0b10011U,
            0b10001U,
            0b01110U,
        },
        // 0x48, H LATIN CAPITAL LETTER H
        {
            0b10001U,
            0b10001U,
            0b10001U,
            0b11111U,
            0b10001U,
            0b10001U,
            0b10001U,
        },
        // 0x49, I LATIN CAPITAL LETTER I
        {
            0b111U,
            0b010U,
            0b010U,
            0b010U,
            0b010U,
            0b010U,
            0b111U,
        },
        // 0x4A, J LATIN CAPITAL LETTER J
        {
            0b00111U,
            0b00010U,
            0b00010U,
            0b00010U,
            0b00010U,
            0b10010U,
            0b01100U,
        },
        // 0x4B, K LATIN CAPITAL LETTER K
        {
            0b10001U,
            0b10010U,
            0b10100U,
            0b11000U,
            0b10100U,
            0b10010U,
            0b10001U,
        },
        // 0x4C, L LATIN CAPITAL LETTER L
        {
            0b10000U,
            0b10000U,
            0b10000U,
            0b10000U,
            0b10000U,
            0b10000U,
            0b11111U,
        },
        // 0x4D, M LATIN CAPITAL LETTER M
        {
            0b10001U,
            0b11011U,
            0b10101U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
        },
        // 0x4E, N LATIN CAPITAL LETTER N
        {
            0b10001U,
            0b10001U,
            0b11001U,
            0b10101U,
            0b10011U,
            0b10001U,
            0b10001U,
        },
        // 0x4F, O LATIN CAPITAL LETTER O
        {
            0b01110U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b01110U,
        },
        // 0x50, P LATIN CAPITAL LETTER P
        {
            0b11110U,
            0b10001U,
            0b10001U,
            0b11110U,
            0b10000U,
            0b10000U,
            0b10000U,
        },
        // 0x51, Q LATIN CAPITAL LETTER Q
        {
            0b01110U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10101U,
            0b10010U,
            0b01101U,
        },
        // 0x52, R LATIN CAPITAL LETTER R
        {
            0b11110U,
            0b10001U,
            0b10001U,
            0b11110U,
            0b10100U,
            0b10010U,
            0b10001U,
        },
        // 0x53, S LATIN CAPITAL LETTER S
        {
            0b01111U,
            0b10000U,
            0b10000U,
            0b01110U,
            0b00001U,
            0b00001U,
            0b11110U,
        },
        // 0x54, T LATIN CAPITAL LETTER T
        {
            0b11111U,
            0b00100U,
            0b00100U,
            0b00100U,
            0b00100U,
            0b00100U,
            0b00100U,
        },
        // 0x55, U LATIN CAPITAL LETTER U
        {
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b01110U,
        },
        // 0x56, V LATIN CAPITAL LETTER V
        {
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b01010U,
            0b00100U,
        },
        // 0x57, W LATIN CAPITAL LETTER W
        {
            0b10001U,
            0b10001U,
            0b10001U,
            0b10101U,
            0b10101U,
            0b11011U,
            0b10001U,
        },
        // 0x58, X LATIN CAPITAL LETTER X
        {
            0b10001U,
            0b10001U,
            0b01010U,
            0b00100U,
            0b01010U,
            0b10001U,
            0b10001U,
        },
        // 0x59, Y LATIN CAPITAL LETTER Y
        {
            0b10001U,
            0b10001U,
            0b01010U,
            0b00100U,
            0b00100U,
            0b00100U,
            0b00100U,
        },
        // 0x5A, Z LATIN CAPITAL LETTER Z
        {
            0b11111U,
            0b00001U,
            0b00010U,
            0b00100U,
            0b01000U,
            0b10000U,
            0b11111U,
        },
        // 0x5B, [ LEFT SQUARE BRACKET
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

    // 0x5C, REVERSE SOLIDUS
    static constexpr std::array<uint8_t, 5U> reverseSolidus{
        0b10000U,
        0b01000U,
        0b00100U,
        0b00010U,
        0b00001U,
    };

    // 0x5D, ] RIGHT SQUARE BRACKET
    static constexpr std::array<uint8_t, 7U> rightSquareBracket{
        0b111U,
        0b001U,
        0b001U,
        0b001U,
        0b001U,
        0b001U,
        0b111U,
    };

    // 0x5E, ^ CIRCUMFLEX ACCENT
    static constexpr std::array<uint8_t, 3U> circumflexAccent{
        0b00100U,
        0b01010U,
        0b10001U,
    };

    // 0x60, ` GRAVE ACCENT
    static constexpr std::array<uint8_t, 3U> graveAccent{
        0b100U,
        0b010U,
        0b001U,
    };

    // 0x61, a LATIN SMALL LETTER A
    static constexpr std::array<uint8_t, 5U> latinSmallLetterA{
        0b01110U,
        0b00001U,
        0b01111U,
        0b10001U,
        0b01111U,
    };

    // 0x62, b LATIN SMALL LETTER B
    static constexpr std::array<uint8_t, 7U> latinSmallLetterB{
        0b10000U,
        0b10000U,
        0b10110U,
        0b11001U,
        0b10001U,
        0b10001U,
        0b11110U,
    };

    // 0x63, c LATIN SMALL LETTER C
    static constexpr std::array<uint8_t, 5U> latinSmallLetterC{
        0b01110U,
        0b10000U,
        0b10000U,
        0b10001U,
        0b01110U,
    };

    // 0x64, d LATIN SMALL LETTER D
    static constexpr std::array<uint8_t, 7U> latinSmallLetterD{
        0b00001U,
        0b00001U,
        0b01101U,
        0b10011U,
        0b10001U,
        0b10001U,
        0b01111U,
    };

    // 0x65, e LATIN SMALL LETTER E
    static constexpr std::array<uint8_t, 5U> latinSmallLetterE{
        0b01110U,
        0b10001U,
        0b11111U,
        0b10000U,
        0b01110U,
    };

    // 0x66, f LATIN SMALL LETTER F
    static constexpr std::array<uint8_t, 7U> latinSmallLetterF{
        0b00110U,
        0b01001U,
        0b01000U,
        0b11100U,
        0b01000U,
        0b01000U,
        0b01000U,
    };

    // 0x67, g LATIN SMALL LETTER G
    static constexpr std::array<uint8_t, 5U> latinSmallLetterG{
        0b01111U,
        0b10001U,
        0b01111U,
        0b00001U,
        0b00110U,
    };

    static constexpr std::array<std::array<uint8_t, 7U>, 5U> latinSmallLetterH_latinSmallLetterL{{
        // 0x68, h LATIN SMALL LETTER H
        {
            0b10000U,
            0b10000U,
            0b10110U,
            0b11001U,
            0b10001U,
            0b10001U,
            0b10001U,
        },
        // 0x69, i LATIN SMALL LETTER I
        {
            0b010U,
            0b000U,
            0b110U,
            0b010U,
            0b010U,
            0b010U,
            0b111U,
        },
        // 0x6A, j LATIN SMALL LETTER J
        {
            0b0001U,
            0b0000U,
            0b0011U,
            0b0001U,
            0b0001U,
            0b1001U,
            0b0110U,
        },
        // 0x6B, k LATIN SMALL LETTER K
        {
            0b10000U,
            0b10000U,
            0b10001U,
            0b10010U,
            0b11100U,
            0b10010U,
            0b10001U,
        },
        // 0x6C, l LATIN SMALL LETTER L
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
        // 0x6D, m LATIN SMALL LETTER M
        {
            0b11010U,
            0b10101U,
            0b10101U,
            0b10001U,
            0b10001U,
        },
        // 0x6E, n LATIN SMALL LETTER N
        {
            0b10110U,
            0b11001U,
            0b10001U,
            0b10001U,
            0b10001U,
        },
        // 0x6F, o LATIN SMALL LETTER O
        {
            0b01110U,
            0b10001U,
            0b10001U,
            0b10001U,
            0b01110U,
        },
        // 0x70, p LATIN SMALL LETTER P
        {
            0b11110U,
            0b10001U,
            0b11110U,
            0b10000U,
            0b10000U,
        },
        // 0x71, q LATIN SMALL LETTER Q
        {
            0b01101U,
            0b10011U,
            0b01111U,
            0b00001U,
            0b00001U,
        },
        // 0x72, r LATIN SMALL LETTER R
        {
            0b10110U,
            0b11001U,
            0b10000U,
            0b10000U,
            0b10000U,
        },
        // 0x73, s LATIN SMALL LETTER S
        {
            0b01110U,
            0b10000U,
            0b01110U,
            0b00001U,
            0b11110U,
        },
    }};

    // 0x74, t LATIN SMALL LETTER T
    static constexpr std::array<uint8_t, 6U> latinSmallLetterT{
        0b01000U,
        0b01000U,
        0b11100U,
        0b01000U,
        0b01001U,
        0b00110U,
    };

    static constexpr std::array<std::array<uint8_t, 5U>, 6U> latinSmallLetterU_latinSmallLetterZ{{
        // 0x75, u LATIN SMALL LETTER U
        {
            0b10001U,
            0b10001U,
            0b10001U,
            0b10011U,
            0b01101U,
        },
        // 0x76, v LATIN SMALL LETTER V
        {
            0b10001U,
            0b10001U,
            0b10001U,
            0b01010U,
            0b00100U,
        },
        // 0x77, w LATIN SMALL LETTER W
        {
            0b10001U,
            0b10001U,
            0b10101U,
            0b10101U,
            0b01010U,
        },
        // 0x78, x LATIN SMALL LETTER X
        {
            0b10001U,
            0b01010U,
            0b00100U,
            0b01010U,
            0b10001U,
        },
        // 0x79, y LATIN SMALL LETTER Y
        {
            0b10001U,
            0b10001U,
            0b01111U,
            0b00001U,
            0b01110U,
        },
        // 0x7A, z LATIN SMALL LETTER Z
        {
            0b11111U,
            0b00010U,
            0b00100U,
            0b01000U,
            0b11111U,
        },
    }};

    static constexpr std::array<std::array<uint8_t, 7U>, 3U> leftCurlyBracket_rightCurlyBracket{{
        // 0x7B, { LEFT CURLY BRACKET
        {
            0b001U,
            0b010U,
            0b010U,
            0b100U,
            0b010U,
            0b010U,
            0b001U,
        },
        // 0x7C, | VERTICAL LINE
        {
            0b1U,
            0b1U,
            0b1U,
            0b1U,
            0b1U,
            0b1U,
            0b1U,
        },
        // 0x7D, } RIGHT CURLY BRACKET
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

    // 0xB0U, ° DEGREE SIGN
    static constexpr std::array<uint8_t, 2U> degreeSign{
        0b11U,
        0b11U,
    };

    // 0xC4, Ä LATIN CAPITAL LETTER A WITH DIAERESIS
    static constexpr std::array<uint8_t, 7U> latinCapitalLetterAWithDiaeresis{
        0b10001U,
        0b01110U,
        0b10001U,
        0b10001U,
        0b11111U,
        0b10001U,
        0b10001U,
    };

    // 0xC5, Å LATIN CAPITAL LETTER A WITH RING ABOVE
    static constexpr std::array<uint8_t, 7U> latinCapitalLetterAWithRingAbove{
        0b01110U,
        0b01110U,
        0b11111U,
        0b10001U,
        0b11111U,
        0b10001U,
        0b10001U,
    };

    // 0xC6, Æ LATIN CAPITAL LETTER AE
    static constexpr std::array<uint8_t, 5U> latinCapitalLetterAe{
        0b011111U,
        0b100100U,
        0b111110U,
        0b100100U,
        0b100111U,
    };

    // 0xD6, Ö LATIN CAPITAL LETTER O WITH DIAERESIS
    static constexpr std::array<uint8_t, 7U> latinCapitalLetterOWithDiaeresis{
        0b10001U,
        0b00000U,
        0b11111U,
        0b10001U,
        0b10001U,
        0b10001U,
        0b01110U,
    };

    // 0xD8, Ø LATIN CAPITAL LETTER O WITH STROKE
    static constexpr std::array<uint8_t, 5U> latinCapitalLetterOWithStroke{
        0b01111U,
        0b10011U,
        0b10101U,
        0b11001U,
        0b11110U,
    };

    // 0xDF, ß LATIN SMALL LETTER SHARP S
    static constexpr std::array<uint8_t, 7U> latinSmallLetterSharpS{
        0b01110U,
        0b10001U,
        0b10001U,
        0b11111U,
        0b10001U,
        0b10001U,
        0b10111U,
    };

    // 0xE4, ä LATIN SMALL LETTER A WITH DIAERESIS
    static constexpr std::array<uint8_t, 6U> latinSmallLetterAWithDiaeresis{
        0b10001U,
        0b01110U,
        0b00001U,
        0b01111U,
        0b10001U,
        0b01111U,
    };

    // 0xE5, å LATIN SMALL LETTER A WITH RING ABOVE
    static constexpr std::array<uint8_t, 7U> latinSmallLetterAWithRingAbove{
        0b01110U,
        0b01110U,
        0b01110U,
        0b00001U,
        0b01111U,
        0b10001U,
        0b01111U,
    };

    // 0xE6, æ LATIN SMALL LETTER AE
    static constexpr std::array<uint8_t, 5U> latinSmallLetterAe{
        0b1111110U,
        0b0001001U,
        0b1111111U,
        0b1001000U,
        0b1111111U,
    };

    // 0xF6, ö LATIN SMALL LETTER O WITH DIAERESIS
    static constexpr std::array<uint8_t, 6U> latinSmallLetterOWithDiaeresis{
        0b10001U,
        0b01110U,
        0b10001U,
        0b10001U,
        0b10001U,
        0b01110U,
    };

    // 0xF8, ø LATIN SMALL LETTER O WITH STROKE
    static constexpr std::array<uint8_t, 5U> latinSmallLetterOWithStroke{
        0b01110U,
        0b10011U,
        0b10101U,
        0b11001U,
        0b01110U,
    };

    // 0xFC, ü LATIN SMALL LETTER U WITH DIAERESIS
    static constexpr std::array<uint8_t, 6U> latinSmallLetterUWithDiaeresis{
        0b10001U,
        0b00000U,
        0b10001U,
        0b10001U,
        0b10011U,
        0b01101U,
    };

    // 0x3C0U, π GREEK SMALL LETTER PI
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
