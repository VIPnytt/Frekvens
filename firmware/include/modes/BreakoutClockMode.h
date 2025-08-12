#pragma once

#include <deque>

#include "config/constants.h"
#include "modules/ModeModule.h"

class BreakoutClockMode : public ModeModule
{
private:
    static constexpr float speed = 7e-4 * ROWS;

    double
        xDec,
        yDec;

    std::deque<uint8_t> paddle;

    tm local;

    uint8_t
        hour,
        minute,
        x,
        y;

    uint16_t deg;

public:
    BreakoutClockMode() : ModeModule("Breakout clock") {};

    void wake() override;
    void handle() override;
};
