#pragma once

#include <deque>

#include "config/constants.h"
#include "modules/ModeModule.h"

class PingPongClockMode : public ModeModule
{
private:
    static constexpr float speed = 6e-4 * COLUMNS;

    bool pending = false;

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
