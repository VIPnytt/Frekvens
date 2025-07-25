#pragma once

#include "modules/ModeModule.h"

class FireworkMode : public ModeModule
{
private:
    unsigned long lastMillis = 0;

    uint8_t
        brightness,
        maxRadius,
        radius,
        rocketX,
        rocketY,
        stage;

    void pad();
    void launching();
    void exploding();
    void fading();

public:
    FireworkMode() : ModeModule("Fireworks") {};

    void handle() override;
};
