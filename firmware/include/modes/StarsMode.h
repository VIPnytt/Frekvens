#pragma once

#if MODE_STARS

#include "config/constants.h"
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

    Star stars[GRID_COLUMNS * GRID_ROWS / (1 << 4)];

public:
    StarsMode() : ModeModule("Stars") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_STARS
