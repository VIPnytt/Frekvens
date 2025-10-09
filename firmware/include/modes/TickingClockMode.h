#pragma once

#include "config/constants.h"

#if MODE_TICKINGCLOCK

#include "modules/ModeModule.h"

class TickingClockMode : public ModeModule
{
private:
    tm local;

    bool pending = false;

    uint8_t
        hour,
        minute,
        secound;

public:
    TickingClockMode() : ModeModule("Ticking clock") {};

    void wake() override;
    void handle() override;
};

#endif // MODE_TICKINGCLOCK
