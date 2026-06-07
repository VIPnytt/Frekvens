#pragma once

#if FONT_MEDIUMWIDE

#include "modules/FontModule.h"

#include <array>

class MediumWideFont final : public FontModule
{
private:
    static constexpr std::array<std::array<uint8_t, 7U>, 10U> digitZero_digitNine{{
        {
            // 0x30, 0 DIGIT ZERO
            0b0111110U,
            0b1100011U,
            0b1100111U,
            0b1101011U,
            0b1110011U,
            0b1100011U,
            0b0111110U,
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
            0b1111110U,
            0b0000011U,
            0b0000011U,
            0b0111110U,
            0b1100000U,
            0b1100011U,
            0b1111111U,
        },
        {
            // 0x33, 3 DIGIT THREE
            0b0111110U,
            0b1100011U,
            0b0000011U,
            0b0011111U,
            0b0000011U,
            0b1100011U,
            0b0111110U,
        },
        {
            // 0x34, 4 DIGIT FOUR
            0b1100011U,
            0b1100011U,
            0b1100011U,
            0b0111111U,
            0b0000011U,
            0b0000011U,
            0b0000011U,
        },
        {
            // 0x35, 5 DIGIT FIVE
            0b1111111U,
            0b1100011U,
            0b1100000U,
            0b1111110U,
            0b0000011U,
            0b1100011U,
            0b0111110U,
        },
        {
            // 0x36, 6 DIGIT SIX
            0b0111110U,
            0b1100011U,
            0b1100000U,
            0b1111110U,
            0b1100011U,
            0b1100011U,
            0b0111110U,
        },
        {
            // 0x37, 7 DIGIT SEVEN
            0b1111111U,
            0b1000011U,
            0b0000111U,
            0b0001100U,
            0b0011000U,
            0b0011000U,
            0b0011000U,
        },
        {
            // 0x38, 8 DIGIT EIGHT
            0b0111110U,
            0b1100011U,
            0b1100011U,
            0b0111110U,
            0b1100011U,
            0b1100011U,
            0b0111110U,
        },
        {
            // 0x39, 9 DIGIT NINE
            0b0111110U,
            0b1100011U,
            0b1100011U,
            0b0111111U,
            0b0000011U,
            0b1100011U,
            0b0111110U,
        },
    }};

    // 0x49, I LATIN CAPITAL LETTER I
    static constexpr std::array<uint8_t, 7U> latinCapitalLetterI{
        0b111U,
        0b010U,
        0b010U,
        0b010U,
        0b010U,
        0b010U,
        0b111U,
    };

    // 0x4F, O LATIN CAPITAL LETTER O
    static constexpr std::array<uint8_t, 7U> latinCapitalLetterO{
        0b011110U,
        0b100001U,
        0b100001U,
        0b100001U,
        0b100001U,
        0b100001U,
        0b011110U,
    };

    // 0x6F, o LATIN SMALL LETTER O
    static constexpr std::array<uint8_t, 5U> latinSmallLetterO{
        0b01110U,
        0b10001U,
        0b10001U,
        0b10001U,
        0b01110U,
    };

public:
    static constexpr std::string_view name{"Medium wide"};

    explicit MediumWideFont() : FontModule(name) {};

    [[nodiscard]] FontModule::Symbol getChar(char32_t character) const override;
};

#endif // FONT_MEDIUMWIDE
