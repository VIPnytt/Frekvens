#pragma once

#include "config/constants.h" // NOLINT(misc-include-cleaner)

#include <array>
#include <ctime>

class ClockHandler
{
private:
    static constexpr std::array<std::array<uint8_t, 5U>, 10U> digits{{
        {
            0b111U,
            0b101U,
            0b101U,
            0b101U,
            0b111U,
        },
        {
            0b010U,
            0b110U,
            0b010U,
            0b010U,
            0b111U,
        },
        {
            0b111U,
            0b001U,
            0b111U,
            0b100U,
            0b111U,
        },
        {
            0b111U,
            0b001U,
            0b111U,
            0b001U,
            0b111U,
        },
        {
            0b101U,
            0b101U,
            0b111U,
            0b001U,
            0b001U,
        },
        {
            0b111U,
            0b100U,
            0b111U,
            0b001U,
            0b111U,
        },
        {
            0b111U,
            0b100U,
            0b111U,
            0b101U,
            0b111U,
        },
        {
            0b111U,
            0b001U,
            0b001U,
            0b001U,
            0b001U,
        },
        {
            0b111U,
            0b101U,
            0b111U,
            0b101U,
            0b111U,
        },
        {
            0b111U,
            0b101U,
            0b111U,
            0b001U,
            0b111U,
        },
    }};

    const bool opaque{true}; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

    int hour{24U};
    int minute{60U};

    tm local{};

    void draw(const std::array<uint8_t, 5U> &digit, uint8_t x, uint8_t y) const;

public:
    explicit ClockHandler(bool opaque = true) : opaque(opaque) {};

    void handle();
    void clear();
};
