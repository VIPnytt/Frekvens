#pragma once

#if MODE_BREAKOUTCLOCK

#include "config/constants.h"
#include "modules/ModeModule.h"

#include <deque>

class BreakoutClockMode final : public ModeModule
{
private:
    static constexpr float speed = 7e-4 * GRID_ROWS;

    float xDec = GRID_COLUMNS / 2.0f;
    float yDec = GRID_ROWS - 2.0f;

    std::deque<uint8_t> paddle = {};

    tm local = {};

    int hour = 24;
    int minute = 60;

    uint8_t x = GRID_COLUMNS / 2;
    uint8_t y = GRID_ROWS - 2;

    uint16_t deg = 90;

public:
    explicit BreakoutClockMode() : ModeModule("Breakout clock") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_BREAKOUTCLOCK
