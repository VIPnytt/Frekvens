#pragma once

#include "config/constants.h"

#if MODE_CIRCLE

#include "config/constants.h"
#include "modules/ModeModule.h"

class CircleMode : public ModeModule
{
private:
    static constexpr float
        x = (GRID_COLUMNS - 1) / 2.0f,
        y = (GRID_ROWS - 1) / 2.0f;

    const uint8_t maxRadius = 3 + ceil((max(GRID_COLUMNS * PITCH_HORIZONTAL / (float)PITCH_VERTICAL, GRID_ROWS / (float)PITCH_HORIZONTAL * PITCH_VERTICAL) / M_SQRT2 + M_SQRT1_2) / 2.0f);

    bool lit = true;

    uint8_t radius = 0;

    unsigned long lastMillis = 0;

public:
    CircleMode() : ModeModule("Circle") {};

    void handle() override;
};

#endif // MODE_CIRCLE
