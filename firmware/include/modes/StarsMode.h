#pragma once

#if MODE_STARS

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <array>

class StarsMode final : public ModeModule
{
private:
    struct Star
    {
        uint8_t brightness = 0;
        uint8_t delay = 0;
        uint8_t x = 0;
        uint8_t y = 0;
        unsigned long lastMillis = 0;
        bool direction = true;
    };

    std::array<Star, GRID_COLUMNS * GRID_ROWS / (1U << 4U)> stars{};

public:
    explicit StarsMode() : ModeModule("Stars") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_STARS
