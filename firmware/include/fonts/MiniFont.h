#pragma once

#if FONT_MINI

#include "modules/FontModule.h"

#include <array>

class MiniFont final : public FontModule
{
private:
    // 0x2B, + PLUS SIGN
    static constexpr std::array<uint8_t, 3U> plusSign{
        0b010U,
        0b111U,
        0b010U,
    };

    // 0x2C, , COMMA
    static constexpr std::array<uint8_t, 2U> comma{
        0b01U,
        0b10U,
    };

    // 0x2D, - HYPHEN-MINUS
    // 0x5F, _ LOW LINE
    static constexpr std::array<uint8_t, 1U> hyphenMinus{0b111U};

    // 0x2E, . FULL STOP
    static constexpr std::array<uint8_t, 1U> fullStop{0b1U};

    static constexpr std::array<std::array<uint8_t, 5U>, 10U> digitZero_digitNine{{
        // 0x30, 0 DIGIT ZERO
        {
            0b111U,
            0b101U,
            0b101U,
            0b101U,
            0b111U,
        },
        // 0x31, 1 DIGIT ONE
        {
            0b010U,
            0b110U,
            0b010U,
            0b010U,
            0b111U,
        },
        // 0x32, 2 DIGIT TWO
        {
            0b111U,
            0b001U,
            0b111U,
            0b100U,
            0b111U,
        },
        // 0x33, 3 DIGIT THREE
        {
            0b111U,
            0b001U,
            0b111U,
            0b001U,
            0b111U,
        },
        // 0x34, 4 DIGIT FOUR
        {
            0b101U,
            0b101U,
            0b111U,
            0b001U,
            0b001U,
        },
        // 0x35, 5 DIGIT FIVE
        {
            0b111U,
            0b100U,
            0b111U,
            0b001U,
            0b111U,
        },
        // 0x36, 6 DIGIT SIX
        {
            0b111U,
            0b100U,
            0b111U,
            0b101U,
            0b111U,
        },
        // 0x37, 7 DIGIT SEVEN
        {
            0b111U,
            0b001U,
            0b001U,
            0b001U,
            0b001U,
        },
        // 0x38, 8 DIGIT EIGHT
        {
            0b111U,
            0b101U,
            0b111U,
            0b101U,
            0b111U,
        },
        // 0x39, 9 DIGIT NINE
        {
            0b111U,
            0b101U,
            0b111U,
            0b001U,
            0b111U,
        },
    }};

    // 0x3A, : COLON
    static constexpr std::array<uint8_t, 3U> colon{
        0b1U,
        0b0U,
        0b1U,
    };

    // 0x3D, = EQUALS SIGN
    static constexpr std::array<uint8_t, 3U> equalsSign{
        0b111U,
        0b000U,
        0b111U,
    };

    static constexpr std::array<std::array<uint8_t, 5U>, 29U> latinCapitalLetterA_rightSquareBracket{{
        // 0x41, A LATIN CAPITAL LETTER A
        {
            0b010U,
            0b101U,
            0b111U,
            0b101U,
            0b101U,
        },
        // 0x42, B LATIN CAPITAL LETTER B
        {
            0b110U,
            0b101U,
            0b110U,
            0b101U,
            0b110U,
        },
        // 0x43, C LATIN CAPITAL LETTER C
        {
            0b111U,
            0b100U,
            0b100U,
            0b100U,
            0b111U,
        },
        // 0x44, D LATIN CAPITAL LETTER D
        {
            0b110U,
            0b101U,
            0b101U,
            0b101U,
            0b110U,
        },
        // 0x45, E LATIN CAPITAL LETTER E
        {
            0b111U,
            0b100U,
            0b110U,
            0b100U,
            0b111U,
        },
        // 0x46, F LATIN CAPITAL LETTER F
        {
            0b111U,
            0b100U,
            0b110U,
            0b100U,
            0b100U,
        },
        // 0x47, G LATIN CAPITAL LETTER G
        {
            0b011U,
            0b100U,
            0b111U,
            0b101U,
            0b110U,
        },
        // 0x48, H LATIN CAPITAL LETTER H
        {
            0b101U,
            0b101U,
            0b111U,
            0b101U,
            0b101U,
        },
        // 0x49, I LATIN CAPITAL LETTER I
        {
            0b1U,
            0b1U,
            0b1U,
            0b1U,
            0b1U,
        },
        // 0x4A, J LATIN CAPITAL LETTER J
        {
            0b001U,
            0b001U,
            0b001U,
            0b101U,
            0b111U,
        },
        // 0x4B, K LATIN CAPITAL LETTER K
        {
            0b101U,
            0b101U,
            0b110U,
            0b111U,
            0b101U,
        },
        // 0x4C, L LATIN CAPITAL LETTER L
        {
            0b100U,
            0b100U,
            0b100U,
            0b100U,
            0b111U,
        },
        // 0x4D, M LATIN CAPITAL LETTER M
        {
            0b10001U,
            0b11011U,
            0b10101U,
            0b10001U,
            0b10001U,
        },
        // 0x4E, N LATIN CAPITAL LETTER N
        {
            0b10001U,
            0b11001U,
            0b10101U,
            0b10011U,
            0b10001U,
        },
        // 0x4F, O LATIN CAPITAL LETTER O
        {
            0b010U,
            0b101U,
            0b101U,
            0b101U,
            0b010U,
        },
        // 0x50, P LATIN CAPITAL LETTER P
        {
            0b110U,
            0b101U,
            0b110U,
            0b100U,
            0b100U,
        },
        // 0x51, Q LATIN CAPITAL LETTER Q
        {
            0b010U,
            0b101U,
            0b101U,
            0b111U,
            0b011U,
        },
        // 0x52, R LATIN CAPITAL LETTER R
        {
            0b110U,
            0b101U,
            0b110U,
            0b110U,
            0b101U,
        },
        // 0x53, S LATIN CAPITAL LETTER S
        {
            0b111U,
            0b100U,
            0b111U,
            0b001U,
            0b111U,
        },
        // 0x54, T LATIN CAPITAL LETTER T
        {
            0b111U,
            0b010U,
            0b010U,
            0b010U,
            0b010U,
        },
        // 0x55, U LATIN CAPITAL LETTER U
        {
            0b101U,
            0b101U,
            0b101U,
            0b101U,
            0b111U,
        },
        // 0x56, V LATIN CAPITAL LETTER V
        {
            0b101U,
            0b101U,
            0b101U,
            0b101U,
            0b010U,
        },
        // 0x57, W LATIN CAPITAL LETTER W
        {
            0b10101U,
            0b10101U,
            0b10101U,
            0b10101U,
            0b01010U,
        },
        // 0x58, X LATIN CAPITAL LETTER X
        {
            0b101U,
            0b101U,
            0b010U,
            0b101U,
            0b101U,
        },
        // 0x59, Y LATIN CAPITAL LETTER Y
        {
            0b101U,
            0b101U,
            0b101U,
            0b010U,
            0b010U,
        },
        // 0x5A, Z LATIN CAPITAL LETTER Z
        {
            0b111U,
            0b001U,
            0b010U,
            0b100U,
            0b111U,
        },
        // 0x5B, [ LEFT SQUARE BRACKET
        {
            0b11U,
            0b10U,
            0b10U,
            0b10U,
            0b11U,
        },
        // 0x5C, REVERSE SOLIDUS
        {
            0b110U,
            0b010U,
            0b010U,
            0b010U,
            0b001U,
        },
        // 0x5D, ] RIGHT SQUARE BRACKET
        {
            0b11U,
            0b01U,
            0b01U,
            0b01U,
            0b11U,
        },
    }};

    // 0x61, a LATIN SMALL LETTER A
    static constexpr std::array<uint8_t, 5U> latinSmallLetterA{
        0b110U,
        0b001U,
        0b011U,
        0b101U,
        0b111U,
    };

    // 0x62, b LATIN SMALL LETTER B
    static constexpr std::array<uint8_t, 4U> latinSmallLetterB{
        0b100U,
        0b110U,
        0b101U,
        0b110U,
    };

    // 0x63, c LATIN SMALL LETTER C
    static constexpr std::array<uint8_t, 3U> latinSmallLetterC{
        0b011U,
        0b100U,
        0b011U,
    };

    // 0x64, d LATIN SMALL LETTER D
    static constexpr std::array<uint8_t, 4U> latinSmallLetterD{
        0b001U,
        0b011U,
        0b101U,
        0b011U,
    };

    static constexpr std::array<std::array<uint8_t, 5U>, 3U> latinSmallLetterE_latinSmallLetterG{{
        // 0x65, e LATIN SMALL LETTER E
        {
            0b010U,
            0b101U,
            0b111U,
            0b100U,
            0b011U,
        },
        // 0x66, f LATIN SMALL LETTER F
        {
            0b011U,
            0b010U,
            0b111U,
            0b010U,
            0b010U,
        },
        // 0x67, g LATIN SMALL LETTER G
        {
            0b011U,
            0b101U,
            0b111U,
            0b111U,
            0b010U,
        },
    }};

    static constexpr std::array<std::array<uint8_t, 4U>, 2U> latinSmallLetterH_latinSmallLetterI{{
        // 0x68, h LATIN SMALL LETTER H
        {
            0b100U,
            0b100U,
            0b110U,
            0b101U,
        },
        // 0x69, i LATIN SMALL LETTER I
        {
            0b1U,
            0b0U,
            0b1U,
            0b1U,
        },
    }};

    // 0x6A, j LATIN SMALL LETTER J
    static constexpr std::array<uint8_t, 5U> latinSmallLetterJ{
        0b01U,
        0b00U,
        0b01U,
        0b01U,
        0b10U,
    };

    static constexpr std::array<std::array<uint8_t, 4U>, 2U> latinSmallLetterK_latinSmallLetterL{{
        // 0x6B, k LATIN SMALL LETTER K
        {
            0b101U,
            0b110U,
            0b110U,
            0b101U,
        },
        // 0x6C, l LATIN SMALL LETTER L
        {
            0b1U,
            0b1U,
            0b1U,
            0b1U,
        },
    }};

    static constexpr std::array<std::array<uint8_t, 3U>, 2U> latinSmallLetterM_latinSmallLetterN{{
        // 0x6D, m LATIN SMALL LETTER M
        {
            0b01010U,
            0b10101U,
            0b10101U,
        },
        // 0x6E, n LATIN SMALL LETTER N
        {
            0b110U,
            0b101U,
            0b101U,
        },
    }};

    static constexpr std::array<std::array<uint8_t, 4U>, 4U> latinSmallLetterO_latinSmallLetterR{{
        // 0x6F, o LATIN SMALL LETTER O
        {
            0b010U,
            0b101U,
            0b101U,
            0b010U,
        },
        // 0x70, p LATIN SMALL LETTER P
        {
            0b110U,
            0b101U,
            0b110U,
            0b100U,
        },
        // 0x71, q LATIN SMALL LETTER Q
        {
            0b011U,
            0b101U,
            0b011U,
            0b011U,
        },
        // 0x72, r LATIN SMALL LETTER R
        {
            0b101U,
            0b110U,
            0b100U,
            0b100U,
        },
    }};

    // 0x73, s LATIN SMALL LETTER S
    static constexpr std::array<uint8_t, 5U> latinSmallLetterS{
        0b011U,
        0b100U,
        0b010U,
        0b001U,
        0b110U,
    };

    // 0x74, t LATIN SMALL LETTER T
    static constexpr std::array<uint8_t, 4U> latinSmallLetterT{
        0b010U,
        0b111U,
        0b010U,
        0b011U,
    };

    static constexpr std::array<std::array<uint8_t, 3U>, 4U> latinSmallLetterU_latinSmallLetterX{{
        // 0x75, u LATIN SMALL LETTER U
        {
            0b101U,
            0b101U,
            0b011U,
        },
        // 0x76, v LATIN SMALL LETTER V
        {
            0b101U,
            0b101U,
            0b010U,
        },
        // 0x77, w LATIN SMALL LETTER W
        {
            0b10101U,
            0b10101U,
            0b01110U,
        },
        // 0x78, x LATIN SMALL LETTER X
        {
            0b101U,
            0b111U,
            0b101U,
        },
    }};

    // 0x79, y LATIN SMALL LETTER Y
    static constexpr std::array<uint8_t, 4U> latinSmallLetterY{
        0b101U,
        0b101U,
        0b010U,
        0b010U,
    };

    // 0x7A, z LATIN SMALL LETTER Z
    static constexpr std::array<uint8_t, 3U> latinSmallLetterZ{
        0b111U,
        0b010U,
        0b111U,
    };

    // 0x7C, | VERTICAL LINE
    static constexpr std::array<uint8_t, 5U> verticalLine{
        0b1U,
        0b1U,
        0b1U,
        0b1U,
        0b1U,
    };

    // 0xB0, ° DEGREE SIGN
    static constexpr std::array<uint8_t, 2U> degreeSign{
        0b11U,
        0b11U,
    };

    // 0x3C0, π GREEK SMALL LETTER PI
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
