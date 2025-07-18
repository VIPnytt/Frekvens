#pragma once

#include "config/constants.h"
#include "modules/ModeModule.h"

class CircleMode : public ModeModule
{
private:
    const uint8_t maxRadius = round(max(COLUMNS * CELL_WIDTH / (float)CELL_HEIGHT, ROWS / (float)CELL_WIDTH * CELL_HEIGHT) / sqrt(2));

    bool lit = true;

    uint8_t radius = 0;

    unsigned long lastMillis = 0;

#if (COLUMNS % 2 == 0)
    const float x = COLUMNS / 2.0 - .5;
#else
    const float x = COLUMNS / 2.0;
#endif
#if (ROWS % 2 == 0)
    const float y = ROWS / 2.0 - .5;
#else
    const float y = ROWS / 2.0;
#endif

public:
    CircleMode() : ModeModule("Circle") {};

    void handle() override;
};
