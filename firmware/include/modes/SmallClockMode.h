#pragma once

#include "config/constants.h"

#if MODE_SMALLCLOCK

#include "modules/ModeModule.h"

class SmallClockMode : public ModeModule
{
private:
    tm local;

    bool pending = false;

    uint8_t
        hour,
        minute;

public:
    SmallClockMode() : ModeModule("Small clock") {};

    void wake() override;
    void handle() override;
};

#endif // MODE_SMALLCLOCK
