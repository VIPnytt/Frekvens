#pragma once

#if MODE_METABALLS

#include "config/constants.h"
#include "modules/ModeModule.h"

class MetaballsMode : public ModeModule
{
private:
    static constexpr float radius = min<float>(GRID_COLUMNS * PITCH_HORIZONTAL / (float)PITCH_VERTICAL,
                                               GRID_ROWS *PITCH_VERTICAL / (float)PITCH_HORIZONTAL) /
                                    PI,
                           radiusSq = radius * radius, speed = 5e-6 * GRID_COLUMNS * GRID_ROWS;

    static constexpr uint8_t multiplier = 1 << 3;

    struct Ball
    {
        float x, y, xVelocity, yVelocity;
    };

    uint8_t contributions[1 << 8];

    Ball balls[GRID_COLUMNS * GRID_ROWS / (1 << 6)];

public:
    MetaballsMode() : ModeModule("Metaballs") {};

    void configure() override;
    void handle() override;
};

#endif // MODE_METABALLS
