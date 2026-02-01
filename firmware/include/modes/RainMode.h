#pragma once

#if MODE_RAIN

#include "config/constants.h"
#include "modules/ModeModule.h"

class RainMode : public ModeModule
{
private:
    struct Drop
    {
        uint8_t delay;
        uint8_t length;
        uint8_t x;
        uint8_t y;

        unsigned long lastMillis = 0;
    };

    Drop drops[GRID_COLUMNS / 3];

public:
    RainMode() : ModeModule("Rain") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_RAIN
