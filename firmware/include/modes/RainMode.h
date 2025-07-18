#pragma once

#include "config/constants.h"
#include "modules/ModeModule.h"

// Ikea Obegränsad stock rain animation:
// 5 drops, length 3-5, and no fading

class RainMode : public ModeModule
{
private:
    struct Drop
    {
        uint8_t
            x,
            y,
            length,
            delay;
        unsigned long lastMillis = 0;
    };

    Drop drops[COLUMNS / 3];

public:
    RainMode() : ModeModule("Rain") {};

    void wake() override;
    void handle() override;
};
