#pragma once

#if FONT_MICRO

#include "modules/FontModule.h"

#include <array>

class MicroFont final : public FontModule
{
private:
    static constexpr std::array<uint8_t, 1> char22 = {0b11}; // 0x22, "

    // 0x27, '
    // 0x2A, *
    // 0x2C, ,
    // 0x2E, .
    // 0x60, `
    // 0xB0, ° DEGREE SIGN
    static constexpr std::array<uint8_t, 1> char27 = {0b1};

    static constexpr std::array<std::array<uint8_t, 3>, 2> chars28 = {{
        // 0x28, (
        {
            0b01,
            0b10,
            0b01,
        },
        // 0x29, (
        {
            0b10,
            0b01,
            0b10,
        },
    }};

    // 0x2B, +
    static constexpr std::array<uint8_t, 3> char2B = {
        0b010,
        0b111,
        0b010,
    };

    // 0x2D, -
    // 0x5F, _
    static constexpr std::array<uint8_t, 1> char2D = {0b111};

    static constexpr std::array<std::array<uint8_t, 3>, 16> chars2F = {{
        // 0x2F, /
        {
            0b001,
            0b010,
            0b100,
        },
        // 0x30, 0
        {
            0b111,
            0b101,
            0b111,
        },
        // 0x31, 1
        {
            0b11,
            0b01,
            0b01,
        },
        // 0x32, 2
        {
            0b110,
            0b010,
            0b011,
        },
        // 0x33, 3
        {
            0b111,
            0b011,
            0b111,
        },
        // 0x34, 4
        {
            0b101,
            0b111,
            0b001,
        },
        // 0x35, 5
        {
            0b011,
            0b010,
            0b110,
        },
        // 0x36, 6
        {
            0b100,
            0b111,
            0b111,
        },
        // 0x37, 7
        {
            0b111,
            0b011,
            0b001,
        },
        // 0x38, 8
        {
            0b111,
            0b111,
            0b111,
        },
        // 0x39, 9
        {
            0b111,
            0b111,
            0b001,
        },
        // 0x3A, :
        {
            0b1,
            0b0,
            0b1,
        },
        // 0x3B, ;
        {
            0b01,
            0b00,
            0b11,
        },
        // 0x3C, <
        {
            0b01,
            0b10,
            0b01,
        },
        // 0x3D, =
        {
            0b111,
            0b000,
            0b111,
        },
        // 0x3E, >
        {
            0b10,
            0b01,
            0b10,
        },
    }};

    static constexpr std::array<std::array<uint8_t, 3>, 26> chars41 = {{
        // 0x41, A
        // 0x61, a
        {
            0b011,
            0b111,
            0b111,
        },
        // 0x42, B
        // 0x62, b
        {
            0b100,
            0b111,
            0b111,
        },
        // 0x43, C
        // 0x63, c
        {
            0b111,
            0b100,
            0b111,
        },
        // 0x44, D
        // 0x64, d
        {
            0b001,
            0b111,
            0b111,
        },
        // 0x45, E
        // 0x65, e
        {
            0b111,
            0b110,
            0b111,
        },
        // 0x46, F
        // 0x66, f
        {
            0b111,
            0b110,
            0b100,
        },
        // 0x47, G
        // 0x67, g
        {
            0b111,
            0b111,
            0b111,
        },
        // 0x48, H
        // 0x68, h
        {
            0b101,
            0b111,
            0b101,
        },
        // 0x49, I
        // 0x69, i
        {
            0b111,
            0b010,
            0b111,
        },
        // 0x4A, J
        // 0x6A, j
        {
            0b111,
            0b010,
            0b110,
        },
        // 0x4B, K
        // 0x6B, k
        {
            0b101,
            0b110,
            0b101,
        },
        // 0x4C, L
        // 0x6C, l
        {
            0b100,
            0b100,
            0b111,
        },
        // 0x4D, M
        // 0x6D, m
        {
            0b111,
            0b111,
            0b101,
        },
        // 0x4E, N
        // 0x6E, n
        {
            0b111,
            0b101,
            0b101,
        },
        // 0x4F, O
        // 0x6F, o
        {
            0b111,
            0b101,
            0b111,
        },
        // 0x50, P
        // 0x70, p
        {
            0b111,
            0b111,
            0b100,
        },
        // 0x51, Q
        // 0x71, q
        {
            0b111,
            0b111,
            0b001,
        },
        // 0x52, R
        // 0x72, r
        {
            0b111,
            0b110,
            0b101,
        },
        // 0x53, S
        // 0x73, s
        {
            0b011,
            0b010,
            0b110,
        },
        // 0x54, T
        // 0x74, t
        {
            0b111,
            0b010,
            0b010,
        },
        // 0x55, U
        // 0x75, u
        {
            0b101,
            0b101,
            0b111,
        },
        // 0x56, V
        // 0x76, v
        {
            0b101,
            0b101,
            0b010,
        },
        // 0x57, W
        // 0x77, w
        {
            0b101,
            0b111,
            0b111,
        },
        // 0x58, X
        // 0x78, x
        {
            0b101,
            0b010,
            0b101,
        },
        // 0x59, Y
        // 0x79, y
        {
            0b101,
            0b111,
            0b010,
        },
        // 0x5A, Z
        // 0x7A, z
        {
            0b110,
            0b010,
            0b011,
        },
    }};

    static constexpr std::array<std::array<uint8_t, 3>, 3> chars5B = {{
        // 0x5B, [
        {
            0b11,
            0b10,
            0b11,
        },
        // 0x5C, REVERSE SOLIDUS
        {
            0b100,
            0b010,
            0b001,
        },
        // 0x5D, ]
        {
            0b11,
            0b01,
            0b11,
        },
    }};

    // 0x5E, ^
    static constexpr std::array<uint8_t, 2> char5E = {
        0b010,
        0b101,
    };

    static constexpr std::array<std::array<uint8_t, 3>, 3> chars7B = {{
        // 0x7B, {
        {
            0b001,
            0b110,
            0b001,
        },
        // 0x7C, |
        {
            0b1,
            0b1,
            0b1,
        },
        // 0x7D, }
        {
            0b100,
            0b011,
            0b100,
        },
    }};

public:
    static constexpr std::string_view name = "Micro";

    explicit MicroFont() : FontModule(name) {};

    [[nodiscard]] FontModule::Symbol getChar(uint32_t character) const override;
};

#endif // FONT_MICRO
