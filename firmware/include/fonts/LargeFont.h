#pragma once

#include "modules/FontModule.h"

class LargeFont : public FontModule
{
private:
    const std::vector<Symbol> ascii = {
        {
            // 0x20, (Space)
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

public:
    LargeFont();

    Symbol getChar(wchar_t character) override;
};

extern LargeFont *FontLarge;
