#pragma once

#include "config/constants.h"

#if MODE_PINGPONG

#include <deque>

#include "modules/ModeModule.h"

class PingPongMode : public ModeModule
{
private:
    static constexpr float speed = 1e-3 * GRID_ROWS;

    float
        xDec = GRID_COLUMNS / 2,
        yDec = GRID_ROWS - 2;

    std::deque<uint8_t>
        paddleT,
        paddleB;

    uint8_t
        targetX = GRID_COLUMNS / 2,
        x = GRID_COLUMNS / 2,
        y = GRID_ROWS - 2;

    uint16_t deg = 90;

    unsigned long lastMillis = 0;

    void predict();

public:
    PingPongMode() : ModeModule("Ping-Pong") {};

    void wake() override;
    void handle() override;
};

#endif // MODE_PINGPONG
