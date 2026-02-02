#pragma once

#if MODE_SMALLCLOCK

#include "modules/ModeModule.h"

class SmallClockMode : public ModeModule
{
private:
    tm local;

    bool pending = false;
    bool ticking = true;

    uint8_t hour = 24;
    uint8_t minute = 60;
    uint8_t second = 60;

    void setTicking(bool _ticking);
    void transmit();

public:
    SmallClockMode() : ModeModule("Small clock") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(const JsonDocument &doc, const char *source) override;
};

#endif // MODE_SMALLCLOCK
