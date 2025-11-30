#pragma once

#include "config/constants.h"

#if MODE_COUNTDOWN

#include <chrono>

#include "modules/ModeModule.h"

class CountdownMode : public ModeModule
{
private:
    bool done = false;

    uint8_t
        upper = 0,
        lower = 0;

    std::chrono::time_point<std::chrono::system_clock> epoch;

    void save();
    void transmit();

public:
    CountdownMode() : ModeModule("Countdown") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(const JsonDocument doc, const char *const source) override;
};

#endif // MODE_COUNTDOWN
