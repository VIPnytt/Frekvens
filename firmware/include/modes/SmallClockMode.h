#pragma once

#if MODE_SMALLCLOCK

#include "modules/ModeModule.h"

class SmallClockMode : public ModeModule
{
private:
    tm local;

    bool pending = false, ticking = true;

    uint8_t hour, minute, second;

    void setTicking(const bool _ticking);
    void transmit();

public:
    SmallClockMode() : ModeModule("Small clock") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(const JsonDocument doc, const char *const source) override;
};

#endif // MODE_SMALLCLOCK
