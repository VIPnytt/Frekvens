#pragma once

#if MODE_NOISE

#include "config/constants.h"
#include "modules/ModeModule.h"

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

    Dot dots[GRID_COLUMNS * GRID_ROWS / 9];

public:
    explicit NoiseMode() : ModeModule("Noise") {};

    void handle() override;
};

#endif // MODE_NOISE
