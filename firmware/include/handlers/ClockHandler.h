#pragma once

#include "config/constants.h" // NOLINT(misc-include-cleaner)

#include <array>
#include <ctime>

class ClockHandler
{
private:
    static constexpr std::array<uint16_t, 10U> digits{
        0b111101101101111U, // 0
        0b010110010010111U, // 1
        0b111001111100111U, // 2
        0b111001111001111U, // 3
        0b101101111001001U, // 4
        0b111100111001111U, // 5
        0b111100111101111U, // 6
        0b111001001001001U, // 7
        0b111101111101111U, // 8
        0b111101111001111U, // 9
    };

    const bool opaque{true}; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

    int hour{24U};
    int minute{60U};

    tm local{};

    void draw(uint16_t bitmap, uint8_t x, uint8_t y) const;

public:
    explicit ClockHandler(bool opaque = true) : opaque(opaque) {};

    void handle();
    void clear() const;
};
