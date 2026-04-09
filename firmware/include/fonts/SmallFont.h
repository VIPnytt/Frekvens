#pragma once

#include "modules/FontModule.h"

#include <array>

class SmallFont final : public FontModule
{
private:
    // 0x21, !
    static constexpr std::array<uint8_t, 7> char21 = {
        0b1,
        0b1,
        0b1,
        0b1,
        0b1,
        0b0,
        0b1,
    };

    // 0x22, "
    static constexpr std::array<uint8_t, 3> char22 = {
        0b101,
        0b101,
        0b101,
    };

    static constexpr std::array<std::array<uint8_t, 7>, 4> chars23 = {{
        // 0x23, #
        {
            0b01010,
            0b01010,
            0b11111,
            0b01010,
            0b11111,
            0b01010,
            0b01010,
        },
        // 0x24, $
        {
            0b00100,
            0b01111,
            0b10100,
            0b01110,
            0b00101,
            0b11110,
            0b00100,
        },
        // 0x25, %
        {
            0b11000,
            0b11001,
            0b00010,
            0b00100,
            0b01000,
            0b10011,
            0b00011,
        },
        // 0x26, &
        {
            0b0110000,
            0b1001000,
            0b1010000,
            0b0100000,
            0b1010101,
            0b1001000,
            0b0110100,
        },
    }};

    // 0x27, '
    static constexpr std::array<uint8_t, 3> char27 = {
        0b11,
        0b01,
        0b10,
    };

    static constexpr std::array<std::array<uint8_t, 7>, 2> chars28 = {{
        // 0x28, (
        {
            0b001,
            0b010,
            0b100,
            0b100,
            0b100,
            0b010,
            0b001,
        },
        // 0x29, )
        {
            0b100,
            0b010,
            0b001,
            0b001,
            0b001,
            0b010,
            0b100,
        },
    }};

    // 0x2A, *
    static constexpr std::array<uint8_t, 2> char2A = {
        0b11,
        0b11,
    };

    // 0x2B, +
    static constexpr std::array<uint8_t, 5> char2B = {
        0b00100,
        0b00100,
        0b11111,
        0b00100,
        0b00100,
    };

    // 0x2C, ,
    static constexpr std::array<uint8_t, 2> char2C = {
        0b01,
        0b10,
    };

    // 0x2D, -
    // 0x5F, _
    static constexpr std::array<uint8_t, 1> char2D = {0b11111};

    // 0x2E, .
    static constexpr std::array<uint8_t, 2> char2E = {
        0b11,
        0b11,
    };

    // 0x2F, /
    static constexpr std::array<uint8_t, 5> char2F = {
        0b00001,
        0b00010,
        0b00100,
        0b01000,
        0b10000,
    };

    static constexpr std::array<std::array<uint8_t, 7>, 10> chars30 = {{
        // 0x30, 0
        {
            0b0110,
            0b1111,
            0b1001,
            0b1001,
            0b1001,
            0b1111,
            0b0110,
        },
        // 0x31, 1
        {
            0b011,
            0b111,
            0b011,
            0b011,
            0b011,
            0b011,
            0b011,
        },
        // 0x32, 2
        {
            0b0110,
            0b1111,
            0b0001,
            0b0100,
            0b1000,
            0b1111,
            0b1111,
        },
        // 0x33, 3
        {
            0b0110,
            0b1111,
            0b0001,
            0b0111,
            0b0001,
            0b1111,
            0b0110,
        },
        // 0x34, 4
        {
            0b1001,
            0b1001,
            0b1111,
            0b0111,
            0b0001,
            0b0001,
            0b0001,
        },
        // 0x35, 5
        {
            0b1111,
            0b1111,
            0b1000,
            0b1110,
            0b0001,
            0b1111,
            0b1110,
        },
        // 0x36, 6
        {
            0b0110,
            0b1111,
            0b1000,
            0b1110,
            0b1001,
            0b1111,
            0b0110,
        },
        // 0x37, 7
        {
            0b1111,
            0b1111,
            0b0010,
            0b0100,
            0b0100,
            0b0100,
            0b0100,
        },
        // 0x38, 8
        {
            0b0110,
            0b1111,
            0b1001,
            0b0110,
            0b1001,
            0b1111,
            0b0110,
        },
        // 0x39, 9
        {
            0b0110,
            0b1111,
            0b1001,
            0b0111,
            0b0001,
            0b1111,
            0b0110,
        },
    }};

    // 0x3A, :
    static constexpr std::array<uint8_t, 5> char3A = {
        0b11,
        0b11,
        0b00,
        0b11,
        0b11,
    };

    // 0x3B, ;
    static constexpr std::array<uint8_t, 6> char3B = {
        0b11,
        0b11,
        0b00,
        0b11,
        0b01,
        0b10,
    };

    // 0x3C, <
    static constexpr std::array<uint8_t, 7> char3C = {
        0b0001,
        0b0010,
        0b0100,
        0b1000,
        0b0100,
        0b0010,
        0b0001,
    };

    // 0x3D, =
    static constexpr std::array<uint8_t, 3> char3D = {
        0b11111,
        0b00000,
        0b11111,
    };

    static constexpr std::array<std::array<uint8_t, 7>, 30> chars3E = {{
        // 0x3E, >
        {
            0b1000,
            0b0100,
            0b0010,
            0b0001,
            0b0010,
            0b0100,
            0b1000,
        },
        // 0x3F, ?
        {
            0b01110,
            0b10001,
            0b00001,
            0b00010,
            0b00100,
            0b00000,
            0b00100,
        },
        // 0x40, @
        {
            0b01110,
            0b10001,
            0b00001,
            0b01101,
            0b10101,
            0b10101,
            0b01110,
        },
        // 0x41, A
        {
            0b01110,
            0b10001,
            0b10001,
            0b10001,
            0b11111,
            0b10001,
            0b10001,
        },
        // 0x42, B
        {
            0b11110,
            0b10001,
            0b10001,
            0b11110,
            0b10001,
            0b10001,
            0b11110,
        },
        // 0x43, C
        {
            0b01110,
            0b10001,
            0b10000,
            0b10000,
            0b10000,
            0b10001,
            0b01110,
        },
        // 0x44, D
        {
            0b11100,
            0b10010,
            0b10001,
            0b10001,
            0b10001,
            0b10010,
            0b11100,
        },
        // 0x45, E
        {
            0b11111,
            0b10000,
            0b10000,
            0b11110,
            0b10000,
            0b10000,
            0b11111,
        },
        // 0x46, F
        {
            0b11111,
            0b10000,
            0b10000,
            0b11100,
            0b10000,
            0b10000,
            0b10000,
        },
        // 0x47, G
        {
            0b01110,
            0b10001,
            0b10000,
            0b10000,
            0b10011,
            0b10001,
            0b01110,
        },
        // 0x48, H
        {
            0b10001,
            0b10001,
            0b10001,
            0b11111,
            0b10001,
            0b10001,
            0b10001,
        },
        // 0x49, I
        {
            0b111,
            0b010,
            0b010,
            0b010,
            0b010,
            0b010,
            0b111,
        },
        // 0x4A, J
        {
            0b00111,
            0b00010,
            0b00010,
            0b00010,
            0b00010,
            0b10010,
            0b01100,
        },
        // 0x4B, K
        {
            0b10001,
            0b10010,
            0b10100,
            0b11000,
            0b10100,
            0b10010,
            0b10001,
        },
        // 0x4C, L
        {
            0b10000,
            0b10000,
            0b10000,
            0b10000,
            0b10000,
            0b10000,
            0b11111,
        },
        // 0x4D, M
        {
            0b10001,
            0b11011,
            0b10101,
            0b10001,
            0b10001,
            0b10001,
            0b10001,
        },
        // 0x4E, N
        {
            0b10001,
            0b10001,
            0b11001,
            0b10101,
            0b10011,
            0b10001,
            0b10001,
        },
        // 0x4F, O
        {
            0b01110,
            0b10001,
            0b10001,
            0b10001,
            0b10001,
            0b10001,
            0b01110,
        },
        // 0x50, P
        {
            0b11110,
            0b10001,
            0b10001,
            0b11110,
            0b10000,
            0b10000,
            0b10000,
        },
        // 0x51, Q
        {
            0b01110,
            0b10001,
            0b10001,
            0b10001,
            0b10101,
            0b10010,
            0b01101,
        },
        // 0x52, R
        {
            0b11110,
            0b10001,
            0b10001,
            0b11110,
            0b10100,
            0b10010,
            0b10001,
        },
        // 0x53, S
        {
            0b011111,
            0b10000,
            0b10000,
            0b01110,
            0b00001,
            0b00001,
            0b11110,
        },
        // 0x54, T
        {
            0b11111,
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b00100,
        },
        // 0x55, U
        {
            0b10001,
            0b10001,
            0b10001,
            0b10001,
            0b10001,
            0b10001,
            0b01110,
        },
        // 0x56, V
        {
            0b10001,
            0b10001,
            0b10001,
            0b10001,
            0b10001,
            0b01010,
            0b00100,
        },
        // 0x57, W
        {
            0b10001,
            0b10001,
            0b10001,
            0b10101,
            0b10101,
            0b11011,
            0b10001,
        },
        // 0x58, X
        {
            0b10001,
            0b10001,
            0b01010,
            0b00100,
            0b01010,
            0b10001,
            0b10001,
        },
        // 0x59, Y
        {
            0b10001,
            0b10001,
            0b01010,
            0b00100,
            0b00100,
            0b00100,
            0b00100,
        },
        // 0x5A, Z
        {
            0b11111,
            0b00001,
            0b00010,
            0b00100,
            0b01000,
            0b10000,
            0b11111,
        },
        // 0x5B, [
        {
            0b111,
            0b100,
            0b100,
            0b100,
            0b100,
            0b100,
            0b111,
        },
    }};

    // 0x5C, REVERSE SOLIDUS
    static constexpr std::array<uint8_t, 5> char5C = {
        0b10000,
        0b01000,
        0b00100,
        0b00010,
        0b00001,
    };

    // 0x5D, ]
    static constexpr std::array<uint8_t, 7> char5D = {
        0b111,
        0b001,
        0b001,
        0b001,
        0b001,
        0b001,
        0b111,
    };

    // 0x5E, ^
    static constexpr std::array<uint8_t, 3> char5E = {
        0b00100,
        0b01010,
        0b10001,
    };

    // 0x60, `
    static constexpr std::array<uint8_t, 3> char60 = {
        0b100,
        0b010,
        0b001,
    };

    // 0x61, a
    static constexpr std::array<uint8_t, 5> char61 = {
        0b01110,
        0b00001,
        0b01111,
        0b10001,
        0b01111,
    };

    // 0x62, b
    static constexpr std::array<uint8_t, 7> char62 = {
        0b10000,
        0b10000,
        0b10110,
        0b11001,
        0b10001,
        0b10001,
        0b11110,
    };

    // 0x63, c
    static constexpr std::array<uint8_t, 5> char63 = {
        0b01110,
        0b10000,
        0b10000,
        0b10001,
        0b01110,
    };

    // 0x64, d
    static constexpr std::array<uint8_t, 7> char64 = {
        0b00001,
        0b00001,
        0b01101,
        0b10011,
        0b10001,
        0b10001,
        0b01111,
    };

    // 0x65, e
    static constexpr std::array<uint8_t, 5> char65 = {
        0b01110,
        0b10001,
        0b11111,
        0b10000,
        0b01110,
    };

    // 0x66, f
    static constexpr std::array<uint8_t, 7> char66 = {
        0b00110,
        0b01001,
        0b01000,
        0b11100,
        0b01000,
        0b01000,
        0b01000,
    };

    // 0x67, g
    static constexpr std::array<uint8_t, 5> char67 = {
        0b01111,
        0b10001,
        0b01111,
        0b00001,
        0b00110,
    };

    static constexpr std::array<std::array<uint8_t, 7>, 5> chars68 = {{
        // 0x68, h
        {
            0b10000,
            0b10000,
            0b10110,
            0b11001,
            0b10001,
            0b10001,
            0b10001,
        },
        // 0x69, i
        {
            0b010,
            0b000,
            0b110,
            0b010,
            0b010,
            0b010,
            0b111,
        },
        // 0x6A, j
        {
            0b0001,
            0b0000,
            0b0011,
            0b0001,
            0b0001,
            0b1001,
            0b0110,
        },
        // 0x6B, k
        {
            0b10000,
            0b10000,
            0b10001,
            0b10010,
            0b11100,
            0b10010,
            0b10001,
        },
        // 0x6C, l
        {
            0b110,
            0b010,
            0b010,
            0b010,
            0b010,
            0b010,
            0b111,
        },
    }};

    static constexpr std::array<std::array<uint8_t, 6>, 7> chars6D = {{
        // 0x6D, m
        {
            0b11010,
            0b10101,
            0b10101,
            0b10001,
            0b10001,
        },
        // 0x6E, n
        {
            0b10110,
            0b11001,
            0b10001,
            0b10001,
            0b10001,
        },
        // 0x6F, o
        {
            0b01110,
            0b10001,
            0b10001,
            0b10001,
            0b01110,
        },
        // 0x70, p
        {
            0b11110,
            0b10001,
            0b11110,
            0b10000,
            0b10000,
        },
        // 0x71, q
        {
            0b01101,
            0b10011,
            0b01111,
            0b00001,
            0b00001,
        },
        // 0x72, r
        {
            0b10110,
            0b11001,
            0b10000,
            0b10000,
            0b10000,
        },
        // 0x73, s
        {
            0b01110,
            0b10000,
            0b01110,
            0b00001,
            0b11110,
        },
    }};

    // 0x74, t
    static constexpr std::array<uint8_t, 6> char74 = {
        0b01000,
        0b01000,
        0b11100,
        0b01000,
        0b01001,
        0b00110,
    };

    static constexpr std::array<std::array<uint8_t, 5>, 6> chars75 = {{
        // 0x75, u
        {
            0b10001,
            0b10001,
            0b10001,
            0b10011,
            0b01101,
        },
        // 0x76, v
        {
            0b10001,
            0b10001,
            0b10001,
            0b01010,
            0b00100,
        },
        // 0x77, w
        {
            0b10001,
            0b10001,
            0b10101,
            0b10101,
            0b01010,
        },
        // 0x78, x
        {
            0b10001,
            0b01010,
            0b00100,
            0b01010,
            0b10001,
        },
        // 0x79, y
        {
            0b10001,
            0b10001,
            0b01111,
            0b00001,
            0b01110,
        },
        // 0x7A, z
        {
            0b11111,
            0b00010,
            0b00100,
            0b01000,
            0b11111,
        },
    }};

    static constexpr std::array<std::array<uint8_t, 7>, 3> chars7B = {{
        // 0x7B, {
        {
            0b001,
            0b010,
            0b010,
            0b100,
            0b010,
            0b010,
            0b001,
        },
        // 0x7C, |
        {
            0b1,
            0b1,
            0b1,
            0b1,
            0b1,
            0b1,
            0b1,
        },
        // 0x7D, }
        {
            0b100,
            0b010,
            0b010,
            0b001,
            0b010,
            0b010,
            0b100,
        },
    }};

    // 0xB0, ° DEGREE SIGN
    static constexpr std::array<uint8_t, 2> charB0 = {
        0b11,
        0b11,
    };

    // 0xC4, Ä LATIN CAPITAL LETTER A WITH DIAERESIS
    static constexpr std::array<uint8_t, 7> charC4 = {
        0b10001,
        0b01110,
        0b10001,
        0b10001,
        0b11111,
        0b10001,
        0b10001,
    };

    // 0xC5, Å LATIN CAPITAL LETTER A WITH RING ABOVE
    static constexpr std::array<uint8_t, 7> charC5 = {
        0b01110,
        0b01110,
        0b11111,
        0b10001,
        0b11111,
        0b10001,
        0b10001,
    };

    // 0xC6, Æ LATIN CAPITAL LETTER AE
    static constexpr std::array<uint8_t, 5> charC6 = {
        0b011111,
        0b100100,
        0b111110,
        0b100100,
        0b100111,
    };

    // 0xD6, Ö LATIN CAPITAL LETTER O WITH DIAERESIS
    static constexpr std::array<uint8_t, 7> charD6 = {
        0b10001,
        0b00000,
        0b11111,
        0b10001,
        0b10001,
        0b10001,
        0b01110,
    };

    // 0xD8, Ø LATIN CAPITAL LETTER O WITH STROKE
    static constexpr std::array<uint8_t, 5> charD8 = {
        0b01111,
        0b10011,
        0b10101,
        0b11001,
        0b11110,
    };

    // 0xDF, ß LATIN SMALL LETTER SHARP S
    static constexpr std::array<uint8_t, 7> charDF = {
        0b01110,
        0b10001,
        0b10001,
        0b11111,
        0b10001,
        0b10001,
        0b10111,
    };

    // 0xE4, ä LATIN SMALL LETTER A WITH DIAERESIS
    static constexpr std::array<uint8_t, 7> charE4 = {
        0b10001,
        0b01110,
        0b00001,
        0b01111,
        0b10001,
        0b01111,
    };

    // 0xE5, å LATIN SMALL LETTER A WITH RING ABOVE
    static constexpr std::array<uint8_t, 7> charE5 = {
        0b01110,
        0b01110,
        0b01110,
        0b00001,
        0b01111,
        0b10001,
        0b01111,
    };

    // 0xE6, æ LATIN SMALL LETTER AE
    static constexpr std::array<uint8_t, 5> charE6 = {
        0b1111110,
        0b0001001,
        0b1111111,
        0b1001000,
        0b1111111,
    };

    // 0xF6, ö LATIN SMALL LETTER O WITH DIAERESIS
    static constexpr std::array<uint8_t, 6> charF6 = {
        0b10001,
        0b01110,
        0b10001,
        0b10001,
        0b10001,
        0b01110,
    };

    // 0xF8, ø LATIN SMALL LETTER O WITH STROKE
    static constexpr std::array<uint8_t, 5> charF8 = {
        0b01110,
        0b10011,
        0b10101,
        0b11001,
        0b01110,
    };

    // 0xFC, ü LATIN SMALL LETTER U WITH DIAERESIS
    static constexpr std::array<uint8_t, 6> charFC = {
        0b10001,
        0b00000,
        0b10001,
        0b10001,
        0b10011,
        0b01101,
    };

    // 0x3C0, π GREEK SMALL LETTER PI
    static constexpr std::array<uint8_t, 5> char3C0 = {
        0b11111,
        0b01010,
        0b01010,
        0b01010,
        0b01010,
    };

public:
    static constexpr std::string_view name = "Small";

    explicit SmallFont() : FontModule(name) {};

    [[nodiscard]] FontModule::Symbol getChar(uint32_t character) const override;
};
