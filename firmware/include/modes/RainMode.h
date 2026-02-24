#pragma once

#if MODE_RAIN

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <array>

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

    std::array<Drop, GRID_COLUMNS / 3> drops{};

public:
    explicit RainMode() : ModeModule("Rain") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_RAIN
