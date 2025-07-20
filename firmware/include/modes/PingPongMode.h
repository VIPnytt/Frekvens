#pragma once

#include <deque>

#include "modules/ModeModule.h"

class PingPongMode : public ModeModule
{
private:
    static constexpr float speed = 0.0005 * ROWS;

    double
        xDec,
        yDec;

    std::deque<uint8_t>
        paddleT,
        paddleB;

    uint8_t
        targetX,
        x,
        y;

    uint16_t deg;

    unsigned long lastMillis = 0;

    void predict();

public:
    PingPongMode() : ModeModule("Ping-Pong") {};

    void wake() override;
    void handle() override;
};
