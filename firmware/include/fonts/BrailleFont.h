#pragma once

#if FONT_BRAILLE

#include "modules/FontModule.h"

#include <array>

class BrailleFont final : public FontModule
{
private:
    static constexpr std::array<uint8_t, 26> chars41{
        // 0x31, 1
        // 0x41, A
        // 0x61, a
        0b100000,
        // 0x32, 2
        // 0x42, B
        // 0x62, b
        0b101000,
        // 0x33, 3
        // 0x43, C
        // 0x63, c
        0b110000,
        // 0x34, 4
        // 0x44, D
        // 0x64, d
        0b110100,
        // 0x35, 5
        // 0x45, E
        // 0x65, e
        0b100100,
        // 0x36, 6
        // 0x46, F
        // 0x66, f
        0b111000,
        // 0x37, 7
        // 0x47, G
        // 0x67, g
        0b111100,
        // 0x38, 8
        // 0x48, H
        // 0x68, h
        0b101100,
        // 0x39, 9
        // 0x49, I
        // 0x69, i
        0b11000,
        // 0x30, 0
        // 0x4A, J
        // 0x6A, j
        0b11100,
        // 0x4B, K
        // 0x6B, k
        0b100010,
        // 0x4C, L
        // 0x6C, l
        0b101010,
        // 0x4D, M
        // 0x6D, m
        0b110010,
        // 0x4E, N
        // 0x6E, n
        0b110110,
        // 0x4F, O
        // 0x6F, o
        0b100110,
        // 0x50, P
        // 0x70, p
        0b111010,
        // 0x51, Q
        // 0x71, q
        0b111110,
        // 0x52, R
        // 0x72, r
        0b101110,
        // 0x53, S
        // 0x73, s
        0b11010,
        // 0x54, T
        // 0x74, t
        0b11110,
        // 0x55, U
        // 0x75, u
        0b100011,
        // 0x56, V
        // 0x76, v
        0b101011,
        // 0x57, W
        // 0x77, w
        0b11101,
        // 0x58, X
        // 0x78, x
        0b110011,
        // 0x59, Y
        // 0x79, y
        0b110111,
        // 0x5A, Z
        // 0x7A, z
        0b100111,
    };

    [[nodiscard]] FontModule::Symbol toSymbol(uint8_t bits) const;

public:
    static constexpr std::string_view name = "Braille";

    explicit BrailleFont() : FontModule(name) {};

    [[nodiscard]] FontModule::Symbol getChar(uint32_t character) const override;
};

#endif // FONT_BRAILLE
