#pragma once

#if MODE_LEAFFALL

#include "config/constants.h"
#include "modules/ModeModule.h"

class LeafFallMode : public ModeModule
{
private:
    struct Leaf
    {
        uint8_t
            x,
            y,
            brightness;
        uint16_t delay;
        unsigned long lastMillis = 0;
    };

    Leaf leaves[GRID_COLUMNS * GRID_ROWS / 20];

public:
    LeafFallMode() : ModeModule("Leaf fall") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_LEAFFALL
