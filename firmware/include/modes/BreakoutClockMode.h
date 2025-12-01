#pragma once

#if MODE_BREAKOUTCLOCK

#include <deque>

#include "config/constants.h"
#include "modules/ModeModule.h"

class BreakoutClockMode : public ModeModule
{
private:
    static constexpr float speed = 7e-4 * GRID_ROWS;

    float
        xDec = GRID_COLUMNS / 2,
        yDec = GRID_ROWS - 2;

    std::deque<uint8_t> paddle;

    tm local;

    uint8_t
        hour,
        minute,
        x = GRID_COLUMNS / 2,
        y = GRID_ROWS - 2;

    uint16_t deg = 90;

public:
    BreakoutClockMode() : ModeModule("Breakout clock") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_BREAKOUTCLOCK
