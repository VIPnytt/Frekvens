#pragma once

#if FONT_BRAILLE

#include "modules/FontModule.h"

#include <array>

class BrailleFont final : public FontModule
{
private:
    static constexpr std::array<uint8_t, 26U> latinLetterA_latinLetterZ{
        // 0x31, 1 DIGIT ONE
        // 0x41, A LATIN CAPITAL LETTER A
        // 0x61, a LATIN SMALL LETTER A
        0b10'00'00U,
        // 0x32, 2 DIGIT TWO
        // 0x42, B LATIN CAPITAL LETTER B
        // 0x62, b LATIN SMALL LETTER B
        0b10'10'00U,
        // 0x33, 3 DIGIT THREE
        // 0x43, C LATIN CAPITAL LETTER C
        // 0x63, c LATIN SMALL LETTER C
        0b11'00'00U,
        // 0x34, 4 DIGIT FOUR
        // 0x44, D LATIN CAPITAL LETTER D
        // 0x64, d LATIN SMALL LETTER D
        0b11'01'00U,
        // 0x35, 5 DIGIT FIVE
        // 0x45, E LATIN CAPITAL LETTER E
        // 0x65, e LATIN SMALL LETTER E
        0b10'01'00U,
        // 0x36, 6 DIGIT SIX
        // 0x46, F LATIN CAPITAL LETTER F
        // 0x66, f LATIN SMALL LETTER F
        0b11'10'00U,
        // 0x37, 7 DIGIT SEVEN
        // 0x47, G LATIN CAPITAL LETTER G
        // 0x67, g LATIN SMALL LETTER G
        0b11'11'00U,
        // 0x38, 8 DIGIT EIGHT
        // 0x48, H LATIN CAPITAL LETTER H
        // 0x68, h LATIN SMALL LETTER H
        0b10'1100U,
        // 0x39, 9 DIGIT NINE
        // 0x49, I LATIN CAPITAL LETTER I
        // 0x69, i LATIN SMALL LETTER I
        0b01'10'00U,
        // 0x30, 0 DIGIT ZERO
        // 0x4A, J LATIN CAPITAL LETTER J
        // 0x6A, j LATIN SMALL LETTER J
        0b11'10'0U,
        // 0x4B, K LATIN CAPITAL LETTER K
        // 0x6B, k LATIN SMALL LETTER K
        0b10'00'10U,
        // 0x4C, L LATIN CAPITAL LETTER L
        // 0x6C, l LATIN SMALL LETTER L
        0b10'10'10U,
        // 0x4D, M LATIN CAPITAL LETTER M
        // 0x6D, m LATIN SMALL LETTER M
        0b11'00'10U,
        // 0x4E, N LATIN CAPITAL LETTER N
        // 0x6E, n LATIN SMALL LETTER N
        0b11'01'10U,
        // 0x4F, O LATIN CAPITAL LETTER O
        // 0x6F, o LATIN SMALL LETTER O
        0b10'01'10U,
        // 0x50, P LATIN CAPITAL LETTER P
        // 0x70, p LATIN SMALL LETTER P
        0b11'10'10U,
        // 0x51, Q LATIN CAPITAL LETTER Q
        // 0x71, q LATIN SMALL LETTER Q
        0b11'11'10U,
        // 0x52, R LATIN CAPITAL LETTER R
        // 0x72, r LATIN SMALL LETTER R
        0b10'11'10U,
        // 0x53, S LATIN CAPITAL LETTER S
        // 0x73, s LATIN SMALL LETTER S
        0b01'11'10U,
        // 0x54, T LATIN CAPITAL LETTER T
        // 0x74, t LATIN SMALL LETTER T
        0b11'110U,
        // 0x55, U LATIN CAPITAL LETTER U
        // 0x75, u LATIN SMALL LETTER U
        0b10'00'11U,
        // 0x56, V LATIN CAPITAL LETTER V
        // 0x76, v LATIN SMALL LETTER V
        0b10'10'11U,
        // 0x57, W LATIN CAPITAL LETTER W
        // 0x77, w LATIN SMALL LETTER W
        0b01'11'01U,
        // 0x58, X LATIN CAPITAL LETTER X
        // 0x78, x LATIN SMALL LETTER X
        0b11'00'11U,
        // 0x59, Y LATIN CAPITAL LETTER Y
        // 0x79, y LATIN SMALL LETTER Y
        0b11'01'11U,
        // 0x5A, Z LATIN CAPITAL LETTER Z
        // 0x7A, z LATIN SMALL LETTER Z
        0b10'01'11U,
    };

    [[nodiscard]] FontModule::Symbol toSymbol(uint8_t bits) const;

public:
    static constexpr std::string_view name{"Braille"};

    explicit BrailleFont() : FontModule(name) {};

    [[nodiscard]] FontModule::Symbol getChar(char32_t character) const override;
};

#endif // FONT_BRAILLE
