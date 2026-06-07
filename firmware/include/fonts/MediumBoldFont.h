#pragma once

#if FONT_MEDIUMBOLD

#include "modules/FontModule.h"

#include <array>
#include <span>

class MediumBoldFont final : public FontModule
{
private:
    static constexpr std::array<std::array<uint8_t, 7U>, 10U> digitZero_digitNine{{
        {
            // 0x30, 0 DIGIT ZERO
            0b011110U,
            0b111111U,
            0b111111U,
            0b111111U,
            0b111111U,
            0b111111U,
            0b011110U,
        },
        {
            // 0x31, 1 DIGIT ONE
            0b0011U,
            0b0111U,
            0b1111U,
            0b0011U,
            0b0011U,
            0b0011U,
            0b0011U,
        },
        {
            // 0x32, 2 DIGIT TWO
            0b011110U,
            0b111111U,
            0b000011U,
            0b001110U,
            0b011000U,
            0b111111U,
            0b111111U,
        },
        {
            // 0x33, 3 DIGIT THREE
            0b011110U,
            0b111111U,
            0b000011U,
            0b001111U,
            0b000011U,
            0b111111U,
            0b011110U,
        },
        {
            // 0x34, 4 DIGIT FOUR
            0b111111U,
            0b111111U,
            0b111111U,
            0b011111U,
            0b000011U,
            0b000011U,
            0b000011U,
        },
        {
            // 0x35, 5 DIGIT FIVE
            0b111111U,
            0b111111U,
            0b110000U,
            0b111110U,
            0b000011U,
            0b111111U,
            0b011110U,
        },
        {
            // 0x36, 6 DIGIT SIX
            0b011110U,
            0b111111U,
            0b110000U,
            0b111110U,
            0b111111U,
            0b111111U,
            0b011110U,
        },
        {
            // 0x37, 7 DIGIT SEVEN
            0b111111U,
            0b111111U,
            0b000110U,
            0b001100U,
            0b001100U,
            0b001100U,
            0b001100U,
        },
        {
            // 0x38, 8 DIGIT EIGHT
            0b011110U,
            0b111111U,
            0b111111U,
            0b011110U,
            0b111111U,
            0b111111U,
            0b011110U,
        },
        {
            // 0x39, 9 DIGIT NINE
            0b011110U,
            0b111111U,
            0b111111U,
            0b011111U,
            0b000011U,
            0b111111U,
            0b011110U,
        },
    }};

    // 0x49, I LATIN CAPITAL LETTER I
    static constexpr std::array<uint8_t, 7U> latinCapitalLetterI{
        0b111111U,
        0b001100U,
        0b001100U,
        0b001100U,
        0b001100U,
        0b001100U,
        0b111111U,
    };

    // 0x4F, O LATIN CAPITAL LETTER O
    static constexpr std::array<uint8_t, 7U> latinCapitalLetterO{
        0b011110U,
        0b111111U,
        0b111111U,
        0b111111U,
        0b111111U,
        0b111111U,
        0b011110U,
    };

    // 0x6F, o LATIN SMALL LETTER O
    static constexpr std::array<uint8_t, 5U> latinSmallLetterO{
        0b01110U,
        0b11111U,
        0b11111U,
        0b11111U,
        0b01110U,
    };

public:
    static constexpr std::string_view name{"Medium bold"};

    explicit MediumBoldFont() : FontModule(name) {};

    [[nodiscard]] FontModule::Symbol getChar(char32_t character) const override;
};

#endif // FONT_MEDIUMBOLD
