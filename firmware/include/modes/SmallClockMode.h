#pragma once

#if MODE_SMALLCLOCK

#include "modules/ModeModule.h"

class SmallClockMode final : public ModeModule
{
private:
    tm local = {};

    bool pending = false;
    bool ticking = true;

    int hour = 24;
    int minute = 60;
    int second = 60;

    void setTicking(bool _ticking);
    void transmit();

public:
    explicit SmallClockMode() : ModeModule("Small clock") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(JsonObjectConst payload, const char *source) override;
};

#endif // MODE_SMALLCLOCK
