#pragma once

#include "config/constants.h"

#if MODE_FIREWORK

#include "modules/ModeModule.h"

class FireworkMode : public ModeModule
{
private:
    unsigned long lastMillis = 0;

    uint8_t
        brightness = UINT8_MAX,
        maxRadius = 1,
        radius = 0,
        rocketX = 0,
        rocketY = GRID_ROWS,
        stage = 0;

    void pad();
    void launching();
    void exploding();
    void fading();

public:
    FireworkMode() : ModeModule("Firework") {};

    void handle() override;
};

#endif // MODE_FIREWORK
