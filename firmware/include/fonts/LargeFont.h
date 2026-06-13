#pragma once

#if FONT_LARGE

#include "modules/FontModule.h"

#include <array>
#include <span>

class LargeFont final : public FontModule
{
private:
    // U+0021 ! EXCLAMATION MARK
    static constexpr std::array<uint8_t, 8U> exclamationMark{
        0b11U,
        0b11U,
        0b11U,
        0b11U,
        0b11U,
        0b00U,
        0b11U,
        0b11U,
    };

    // U+0049 I LATIN CAPITAL LETTER I
    static constexpr std::array<uint8_t, 8U> latinCapitalLetterI{
        0b111111U,
        0b001100U,
        0b001100U,
        0b001100U,
        0b001100U,
        0b001100U,
        0b001100U,
        0b111111U,
    };

    // U+0052 R LATIN CAPITAL LETTER R
    static constexpr std::array<uint8_t, 8U> latinCapitalLetterR{
        0b111110U,
        0b110011U,
        0b110011U,
        0b110010U,
        0b111100U,
        0b110011U,
        0b110011U,
        0b110011U,
    };

    // U+0055 U LATIN CAPITAL LETTER U
    static constexpr std::array<uint8_t, 8U> latinCapitalLetterU{
        0b110011U,
        0b110011U,
        0b110011U,
        0b110011U,
        0b110011U,
        0b110011U,
        0b111111U,
        0b011111U,
    };

    // U+03C0 π GREEK SMALL LETTER PI
    static constexpr std::array<uint8_t, 8U> greekSmallLetterPi{
        0b11111111U,
        0b11111111U,
        0b00100100U,
        0b00100100U,
        0b00100100U,
        0b00100100U,
        0b00100101U,
        0b11000010U,
    };

public:
    static constexpr std::string_view name{"Large"};

    explicit LargeFont() : FontModule(name) {};

    [[nodiscard]] FontModule::Symbol getChar(char32_t character) const override;
};

#endif // FONT_LARGE
