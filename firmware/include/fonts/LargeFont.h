#pragma once

#include "modules/FontModule.h"

#include <array>
#include <span>

class LargeFont final : public FontModule
{
private:
    // 0x21, !
    static constexpr std::array<uint8_t, 8> _21 = {
        0b11,
        0b11,
        0b11,
        0b11,
        0b11,
        0b00,
        0b11,
        0b11,
    };

    // 0x49, I
    static constexpr std::array<uint8_t, 8> _49 = {
        0b111111,
        0b001100,
        0b001100,
        0b001100,
        0b001100,
        0b001100,
        0b001100,
        0b111111,
    };

    // 0x52, R
    static constexpr std::array<uint8_t, 8> _52 = {
        0b111110,
        0b110011,
        0b110011,
        0b110010,
        0b111100,
        0b110011,
        0b110011,
        0b110011,
    };

    // 0x55, U
    static constexpr std::array<uint8_t, 8> _55 = {
        0b110011,
        0b110011,
        0b110011,
        0b110011,
        0b110011,
        0b110011,
        0b111111,
        0b011111,
    };

    // 0x3C0, π GREEK SMALL LETTER PI
    static constexpr std::array<uint8_t, 8> _3C0 = {
        0b11111111,
        0b11111111,
        0b00100100,
        0b00100100,
        0b00100100,
        0b00100100,
        0b00100101,
        0b11000010,
    };

public:
    static constexpr std::string_view name = "Large";

    explicit LargeFont() : FontModule(name) {};

    [[nodiscard]] FontModule::Symbol getChar(uint32_t character) const override;
};
