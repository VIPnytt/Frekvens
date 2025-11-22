#pragma once

#include "config/constants.h"

#if MODE_PINGPONGCLOCK

#include <deque>

#include "modules/ModeModule.h"

class PingPongClockMode : public ModeModule
{
private:
    static constexpr float speed = 1e-3 * GRID_COLUMNS;

    bool pending = false;

    float
        xDec,
        yDec;

    std::deque<uint8_t>
        paddleL,
        paddleR;

    tm local;

    uint8_t
        hour,
        minute,
        targetY = GRID_ROWS / 2,
        x = GRID_COLUMNS - 2,
        y = GRID_ROWS / 2;

    uint16_t deg = 180;

    unsigned long lastMillis = 0;

    void predict();

public:
    PingPongClockMode() : ModeModule("Ping-Pong clock") {};

    void wake() override;
    void handle() override;
};

#endif // MODE_PINGPONGCLOCK
