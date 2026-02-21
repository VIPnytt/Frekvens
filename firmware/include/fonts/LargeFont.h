#pragma once

#include "modules/FontModule.h"

class LargeFont final : public FontModule
{
private:
    inline static const std::vector<Symbol> ascii = {
        {
            // 0x20, SPACE
            {},
            6,
            0,
        },
        {
            // 0x21, !
            {
                0b11,
                0b11,
                0b11,
                0b11,
                0b11,
                0b00,
                0b11,
                0b11,
            },
            0,
            0,
        },
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {
            // 0x49, I
            {
                0b111111,
                0b001100,
                0b001100,
                0b001100,
                0b001100,
                0b001100,
                0b001100,
                0b111111,
            },
            0,
            0,
        },
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {
            // 0x52, R
            {
                0b111110,
                0b110011,
                0b110011,
                0b110010,
                0b111100,
                0b110011,
                0b110011,
                0b110011,
            },
            0,
            0,
        },
        {},
        {},
        {
            // 0x55, U
            {
                0b110011,
                0b110011,
                0b110011,
                0b110011,
                0b110011,
                0b110011,
                0b111111,
                0b011111,
            },
            0,
            0,
        },
    };

    const std::vector<SymbolExtended> unicode = {
        {
            0x3C0, // π GREEK SMALL LETTER PI
            {
                {
                    0b11111111,
                    0b11111111,
                    0b00100100,
                    0b00100100,
                    0b00100100,
                    0b00100100,
                    0b00100101,
                    0b11000010,
                },
                0,
                0,
            },
        },
    };

public:
    explicit LargeFont();

    [[nodiscard]] Symbol getChar(uint32_t character) const override;
};

extern LargeFont *FontLarge;
