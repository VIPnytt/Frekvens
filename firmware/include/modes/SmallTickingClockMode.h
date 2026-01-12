#pragma once

#if MODE_SMALLTICKINGCLOCK

#include "modules/ModeModule.h"

class SmallTickingClockMode : public ModeModule
{
private:
    tm local;

    bool pending = false;

    uint8_t
        hour,
        minute,
        second;

    bool ticking = true;

    void setTicking(const bool _ticking);
    void transmit();

public:
    SmallTickingClockMode() : ModeModule("Small ticking clock") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(const JsonDocument doc, const char *const source) override;
};

#endif // MODE_SMALLETICKINGCLOCK
