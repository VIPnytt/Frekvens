#pragma once

#include "modules/FontModule.h"

#include <array>
#include <span>

class MediumBoldFont final : public FontModule
{
private:
    static constexpr std::array<std::array<uint8_t, 7>, 10> chars30 = {{
        {
            // 0x30, 0
            0b011110,
            0b111111,
            0b111111,
            0b111111,
            0b111111,
            0b111111,
            0b011110,
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
            0b011110,
            0b111111,
            0b000011,
            0b001110,
            0b011000,
            0b111111,
            0b111111,
        },
        {
            // 0x33, 3
            0b011110,
            0b111111,
            0b000011,
            0b001111,
            0b000011,
            0b111111,
            0b011110,
        },
        {
            // 0x34, 4
            0b111111,
            0b111111,
            0b111111,
            0b011111,
            0b000011,
            0b000011,
            0b000011,
        },
        {
            // 0x35, 5
            0b111111,
            0b111111,
            0b110000,
            0b111110,
            0b000011,
            0b111111,
            0b011110,
        },
        {
            // 0x36, 6
            0b011110,
            0b111111,
            0b110000,
            0b111110,
            0b111111,
            0b111111,
            0b011110,
        },
        {
            // 0x37, 7
            0b111111,
            0b111111,
            0b000110,
            0b001100,
            0b001100,
            0b001100,
            0b001100,
        },
        {
            // 0x38, 8
            0b011110,
            0b111111,
            0b111111,
            0b011110,
            0b111111,
            0b111111,
            0b011110,
        },
        {
            // 0x39, 9
            0b011110,
            0b111111,
            0b111111,
            0b011111,
            0b000011,
            0b111111,
            0b011110,
        },
    }};

    // 0x49, I
    static constexpr std::array<uint8_t, 7> char49 = {
        0b111111,
        0b001100,
        0b001100,
        0b001100,
        0b001100,
        0b001100,
        0b111111,
    };

    // 0x4F, O
    static constexpr std::array<uint8_t, 7> char4F = {
        0b011110,
        0b111111,
        0b111111,
        0b111111,
        0b111111,
        0b111111,
        0b011110,
    };

    // 0x6F, o
    static constexpr std::array<uint8_t, 5> char6F = {
        0b01110,
        0b11111,
        0b11111,
        0b11111,
        0b01110,
    };

public:
    static constexpr std::string_view name = "Medium bold";

    explicit MediumBoldFont() : FontModule(name) {};

    [[nodiscard]] FontModule::Symbol getChar(uint32_t character) const override;
};
