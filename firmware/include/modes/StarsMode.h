#pragma once

#if MODE_STARS

#include "config/constants.h"
#include "modules/ModeModule.h"

class StarsMode final : public ModeModule
{
private:
    struct Star
    {
        uint8_t brightness = 0;
        uint8_t delay = 0;
        uint8_t x = 0;
        uint8_t y = 0;
        unsigned long lastMillis = 0;
        bool direction = true;
    };

    Star stars[GRID_COLUMNS * GRID_ROWS / (1 << 4)];

public:
    explicit StarsMode() : ModeModule("Stars") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_STARS
