#pragma once

#include "config/constants.h"

#if MODE_STARS

#include "modules/ModeModule.h"

class StarsMode : public ModeModule
{
private:
    struct Star
    {
        uint8_t
            x = 0,
            y = 0,
            brightness = 0,
            delay = 0;
        unsigned long lastMillis = 0;
        bool direction = true;
    };

    Star stars[COLUMNS * ROWS / 15];

public:
    StarsMode() : ModeModule("Stars") {};

    void wake() override;
    void handle() override;
};

#endif // MODE_STARS
