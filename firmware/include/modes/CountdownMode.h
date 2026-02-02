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

protected:
    ~CountdownMode() = default;

public:
    CountdownMode() : ModeModule("Countdown") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(const JsonDocument &doc, const char *source) override;
};

#endif // MODE_COUNTDOWN
