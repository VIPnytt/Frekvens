#pragma once

#if FONT_MEDIUMWIDE

#include "modules/FontModule.h"

#include <array>

class MediumWideFont final : public FontModule
{
private:
    static constexpr std::array<std::array<uint8_t, 7U>, 10U> digitZero_digitNine{{
        {
            // U+0030 0 DIGIT ZERO
            0b0111110U,
            0b1100011U,
            0b1100111U,
            0b1101011U,
            0b1110011U,
            0b1100011U,
            0b0111110U,
        },
        {
            // U+0031 1 DIGIT ONE
            0b0011U,
            0b0111U,
            0b1111U,
            0b0011U,
            0b0011U,
            0b0011U,
            0b0011U,
        },
        {
            // U+0032 2 DIGIT TWO
            0b1111110U,
            0b0000011U,
            0b0000011U,
            0b0111110U,
            0b1100000U,
            0b1100011U,
            0b1111111U,
        },
        {
            // U+0033 3 DIGIT THREE
            0b0111110U,
            0b1100011U,
            0b0000011U,
            0b0011111U,
            0b0000011U,
            0b1100011U,
            0b0111110U,
        },
        {
            // U+0034 4 DIGIT FOUR
            0b1100011U,
            0b1100011U,
            0b1100011U,
            0b0111111U,
            0b0000011U,
            0b0000011U,
            0b0000011U,
        },
        {
            // U+0035 5 DIGIT FIVE
            0b1111111U,
            0b1100011U,
            0b1100000U,
            0b1111110U,
            0b0000011U,
            0b1100011U,
            0b0111110U,
        },
        {
            // U+0036 6 DIGIT SIX
            0b0111110U,
            0b1100011U,
            0b1100000U,
            0b1111110U,
            0b1100011U,
            0b1100011U,
            0b0111110U,
        },
        {
            // U+0037 7 DIGIT SEVEN
            0b1111111U,
            0b1000011U,
            0b0000111U,
            0b0001100U,
            0b0011000U,
            0b0011000U,
            0b0011000U,
        },
        {
            // U+0038 8 DIGIT EIGHT
            0b0111110U,
            0b1100011U,
            0b1100011U,
            0b0111110U,
            0b1100011U,
            0b1100011U,
            0b0111110U,
        },
        {
            // U+0039 9 DIGIT NINE
            0b0111110U,
            0b1100011U,
            0b1100011U,
            0b0111111U,
            0b0000011U,
            0b1100011U,
            0b0111110U,
        },
    }};

    // U+0049 I LATIN CAPITAL LETTER I
    static constexpr std::array<uint8_t, 7U> latinCapitalLetterI{
        0b111U,
        0b010U,
        0b010U,
        0b010U,
        0b010U,
        0b010U,
        0b111U,
    };

    // U+004F O LATIN CAPITAL LETTER O
    static constexpr std::array<uint8_t, 7U> latinCapitalLetterO{
        0b011110U,
        0b100001U,
        0b100001U,
        0b100001U,
        0b100001U,
        0b100001U,
        0b011110U,
    };

    // U+006F o LATIN SMALL LETTER O
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
