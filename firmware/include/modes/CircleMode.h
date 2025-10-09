#pragma once

#include "config/constants.h"

#if MODE_CIRCLE

#include "config/constants.h"
#include "modules/ModeModule.h"

class CircleMode : public ModeModule
{
private:
    static constexpr float
        x = (COLUMNS - 1) / 2.0,
        y = (ROWS - 1) / 2.0;

    const uint8_t maxRadius = ceil(max(COLUMNS * CELL_WIDTH / (float)CELL_HEIGHT, ROWS / (float)CELL_WIDTH * CELL_HEIGHT) / M_SQRT2 + M_SQRT1_2);

    bool lit = true;

    uint8_t radius = 0;

    unsigned long lastMillis = 0;

public:
    CircleMode() : ModeModule("Circle") {};

    void handle() override;
};

#endif // MODE_CIRCLE
