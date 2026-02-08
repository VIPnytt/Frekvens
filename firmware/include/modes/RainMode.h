#pragma once

#if MODE_RAIN

#include "config/constants.h"
#include "modules/ModeModule.h"

class RainMode final : public ModeModule
{
private:
    struct Drop
    {
        uint8_t delay = INT8_MAX;
        uint8_t length = 3;
        uint8_t x = 0;
        uint8_t y = 0;

        unsigned long lastMillis = 0;
    };

    Drop drops[GRID_COLUMNS / 3];

public:
    explicit RainMode() : ModeModule("Rain") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_RAIN
