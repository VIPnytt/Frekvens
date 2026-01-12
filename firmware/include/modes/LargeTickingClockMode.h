#pragma once

#if MODE_LARGETICKINGCLOCK

#include "modules/ModeModule.h"

class LargeTickingClockMode : public ModeModule
{
private:
    tm local;

    bool pending = false;

    uint8_t
        hour,
        minute,
        second = 0;

    bool ticking = true;

    void setTicking(const bool _ticking);
    void transmit();

public:
    LargeTickingClockMode() : ModeModule("Large ticking clock") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(const JsonDocument doc, const char *const source) override;
};

#endif // MODE_LARGETICKINGCLOCK
