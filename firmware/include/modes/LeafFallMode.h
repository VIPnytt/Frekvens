#pragma once

#include "config/constants.h"

#if MODE_LEAFFALL

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

    Leaf leaves[COLUMNS * ROWS / 20];

public:
    LeafFallMode() : ModeModule("Leaf fall") {};

    void wake() override;
    void handle() override;
};

#endif // MODE_LEAFFALL
