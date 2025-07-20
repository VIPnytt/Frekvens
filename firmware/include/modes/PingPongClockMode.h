#pragma once

#include <deque>
#include <vector>

#include "modules/ModeModule.h"

class PingPongClockMode : public ModeModule
{
private:
    static constexpr float speed = 0.0006 * COLUMNS;

    double
        xDec,
        yDec;

    std::deque<uint8_t>
        paddleL,
        paddleR;

    tm local;

    uint8_t
        hour,
        minute,
        targetY,
        x,
        y;

    uint16_t deg;

    unsigned long lastMillis = 0;

    void predict();

public:
    PingPongClockMode() : ModeModule("Ping-Pong clock") {};

    void wake() override;
    void handle() override;
};
