#pragma once

#if FONT_MEDIUMWIDE

#include "modules/FontModule.h"

#include <array>

class MediumWideFont final : public FontModule
{
private:
    static constexpr std::array<std::array<uint8_t, 7>, 10> chars30 = {{
        {
            // 0x30, 0
            0b0111110,
            0b1100011,
            0b1100111,
            0b1101011,
            0b1110011,
            0b1100011,
            0b0111110,
        },
        {
            // 0x31, 1
            0b0011,
            0b0111,
            0b1111,
            0b0011,
            0b0011,
            0b0011,
            0b0011,
        },
        {
            // 0x32, 2
            0b1111110,
            0b0000011,
            0b0000011,
            0b0111110,
            0b1100000,
            0b1100011,
            0b1111111,
        },
        {
            // 0x33, 3
            0b0111110,
            0b1100011,
            0b0000011,
            0b0011111,
            0b0000011,
            0b1100011,
            0b0111110,
        },
        {
            // 0x34, 4
            0b1100011,
            0b1100011,
            0b1100011,
            0b0111111,
            0b0000011,
            0b0000011,
            0b0000011,
        },
        {
            // 0x35, 5
            0b1111111,
            0b1100011,
            0b1100000,
            0b1111110,
            0b0000011,
            0b1100011,
            0b0111110,
        },
        {
            // 0x36, 6
            0b0111110,
            0b1100011,
            0b1100000,
            0b1111110,
            0b1100011,
            0b1100011,
            0b0111110,
        },
        {
            // 0x37, 7
            0b1111111,
            0b1000011,
            0b0000111,
            0b0001100,
            0b0011000,
            0b0011000,
            0b0011000,
        },
        {
            // 0x38, 8
            0b0111110,
            0b1100011,
            0b1100011,
            0b0111110,
            0b1100011,
            0b1100011,
            0b0111110,
        },
        {
            // 0x39, 9
            0b0111110,
            0b1100011,
            0b1100011,
            0b0111111,
            0b0000011,
            0b1100011,
            0b0111110,
        },
    }};

    // 0x49, I
    static constexpr std::array<uint8_t, 7> char49 = {
        0b111,
        0b010,
        0b010,
        0b010,
        0b010,
        0b010,
        0b111,
    };

    // 0x4F, O
    static constexpr std::array<uint8_t, 7> char4F = {
        0b011110,
        0b100001,
        0b100001,
        0b100001,
        0b100001,
        0b100001,
        0b011110,
    };

    // 0x6F, o
    static constexpr std::array<uint8_t, 5> char6F = {
        0b01110,
        0b10001,
        0b10001,
        0b10001,
        0b01110,
    };

public:
    static constexpr std::string_view name = "Medium wide";

    explicit MediumWideFont() : FontModule(name) {};

    [[nodiscard]] FontModule::Symbol getChar(uint32_t character) const override;
};

#endif // FONT_MEDIUMWIDE
