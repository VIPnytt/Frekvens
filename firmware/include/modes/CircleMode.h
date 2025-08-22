#pragma once

#include "config/constants.h"
#include "modules/ModeModule.h"

class CircleMode : public ModeModule
{
private:
    const uint8_t maxRadius = round(max(COLUMNS * CELL_WIDTH / (float)CELL_HEIGHT, ROWS / (float)CELL_WIDTH * CELL_HEIGHT) / M_SQRT2);

    bool lit = true;

    uint8_t radius = 0;

    unsigned long lastMillis = 0;

#if COLUMNS % 2
    static constexpr float x = COLUMNS / 2.0;
#else
    static constexpr float x = COLUMNS / 2.0 - .5;
#endif // COLUMNS % 2
#if ROWS % 2
    static constexpr float y = ROWS / 2.0;
#else
    static constexpr float y = ROWS / 2.0 - .5;
#endif // ROWS % 2

public:
    CircleMode() : ModeModule("Circle") {};

    void handle() override;
};
