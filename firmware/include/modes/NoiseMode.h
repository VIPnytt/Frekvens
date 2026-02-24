#pragma once

#if MODE_NOISE

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <array>

class NoiseMode final : public ModeModule
{
private:
    struct Dot
    {
        uint8_t x = 0;
        uint8_t y = 0;
        uint8_t delay = 0;
        unsigned long lastMillis = 0;
    };

    std::array<Dot, GRID_COLUMNS * GRID_ROWS / 9> dots{};

public:
    explicit NoiseMode() : ModeModule("Noise") {};

    void handle() override;
};

#endif // MODE_NOISE
