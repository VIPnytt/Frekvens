#pragma once

#include <deque>
#include <vector>

#include "modules/ModeModule.h"

class BreakoutClockMode : public ModeModule
{
private:
    static constexpr float speed = 0.009;

    double
        xDec,
        yDec;

    std::deque<uint8_t> paddle;

    tm local;

    uint8_t
        hour,
        min,
        x,
        y;

    uint16_t deg;

public:
    BreakoutClockMode() : ModeModule("Breakout clock") {};

    void wake() override;
    void handle() override;
};
