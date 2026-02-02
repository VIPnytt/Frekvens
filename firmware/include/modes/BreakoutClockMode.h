#pragma once

#if MODE_BREAKOUTCLOCK

#include "config/constants.h"
#include "modules/ModeModule.h"

#include <deque>

class BreakoutClockMode : public ModeModule
{
private:
    static constexpr float speed = 7e-4 * GRID_ROWS;

    float xDec = GRID_COLUMNS / 2;
    float yDec = GRID_ROWS - 2;

    std::deque<uint8_t> paddle;

    tm local;

    uint8_t hour = 24;
    uint8_t minute = 60;
    uint8_t x = GRID_COLUMNS / 2;
    uint8_t y = GRID_ROWS - 2;

    uint16_t deg = 90;

public:
    BreakoutClockMode() : ModeModule("Breakout clock") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_BREAKOUTCLOCK
