#pragma once

#if MODE_COUNTDOWN

#include "modules/ModeModule.h"

#include <chrono>

class CountdownMode final : public ModeModule
{
private:
    bool done = false;

    uint8_t lower = 0;
    uint8_t upper = 0;

    std::chrono::time_point<std::chrono::system_clock> epoch = {};

    void save();
    void transmit();

public:
    explicit CountdownMode() : ModeModule("Countdown") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(JsonObjectConst payload, const char *source) override;
};

#endif // MODE_COUNTDOWN
