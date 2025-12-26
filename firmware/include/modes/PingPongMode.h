#pragma once

#if MODE_PINGPONG

#include <deque>

#include "config/constants.h"
#include "modules/ModeModule.h"

class PingPongMode : public ModeModule
{
private:
    static constexpr float speed = 1e-3 * GRID_ROWS;

    float
        xDec = GRID_COLUMNS / 2,
        yDec = GRID_ROWS - 2;

    std::deque<uint8_t>
        paddleB,
        paddleT;

    uint8_t
        x = GRID_COLUMNS / 2,
        y = GRID_ROWS - 2;

    uint16_t deg = 90;

    unsigned long lastMillis = 0;

public:
    PingPongMode() : ModeModule("Ping-Pong") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_PINGPONG
