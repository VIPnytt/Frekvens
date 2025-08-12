#pragma once

#include "config/constants.h"
#include "modules/ModeModule.h"

class MetaballsMode : public ModeModule
{
protected:
    static constexpr float speed = 5e-5 * COLUMNS * ROWS;

    struct Ball
    {
        float
            x,
            y,
            xVelocity,
            yVelocity;
    };

    float radiusSq;

    Ball balls[COLUMNS * ROWS / 40];

public:
    MetaballsMode() : ModeModule("Metaballs") {};

    void wake() override;
    void handle() override;
};
