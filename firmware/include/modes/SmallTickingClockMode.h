#pragma once

#include "config/constants.h"

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
        secound;

public:
    SmallTickingClockMode() : ModeModule("Small ticking clock") {};

    void wake() override;
    void handle() override;
};

#endif // MODE_SMALLETICKINGCLOCK
