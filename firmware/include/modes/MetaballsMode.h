#pragma once

#include "config/constants.h"
#include "modules/ModeModule.h"

class MetaballsMode : public ModeModule
{
protected:
    static constexpr float speed = 5e-5 * COLUMNS * ROWS;

    static constexpr uint8_t multiplier = 1 << 3;

    struct Ball
    {
        float
            x,
            y,
            xVelocity,
            yVelocity;
    };

    float
        radius,
        radiusSq;

    uint8_t contributions[1 << 8];

    Ball balls[COLUMNS * ROWS / 50];

public:
    MetaballsMode() : ModeModule("Metaballs") {};

    void setup() override;
    void wake() override;
    void handle() override;
};
