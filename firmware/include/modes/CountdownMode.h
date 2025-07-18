#pragma once

#include <chrono>

#include "modules/ModeModule.h"

class CountdownMode : public ModeModule
{
private:
    bool done = false;

    uint8_t
        upper,
        lower,
        _upper,
        _lower;

    std::chrono::time_point<std::chrono::system_clock> target;

    void save();
    void transmit();

public:
    CountdownMode() : ModeModule("Countdown") {};

    void setup() override;
    void wake() override;
    void handle() override;
    void receiverHook(const JsonDocument doc) override;
};
