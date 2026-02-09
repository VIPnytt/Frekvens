#pragma once

#if MODE_LEAFFALL

#include "config/constants.h"
#include "modules/ModeModule.h"

class LeafFallMode final : public ModeModule
{
private:
    struct Leaf
    {
        uint8_t x = 0;
        uint8_t y = 0;
        uint8_t brightness = INT8_MAX;
        uint16_t delay = UINT8_MAX;
        unsigned long lastMillis = 0;
    };

    Leaf leaves[GRID_COLUMNS * GRID_ROWS / 20];

public:
    explicit LeafFallMode() : ModeModule("Leaf fall") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_LEAFFALL
