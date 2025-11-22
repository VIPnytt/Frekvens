#pragma once

#include "config/constants.h"

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
        secound = 0;

public:
    LargeTickingClockMode() : ModeModule("Large ticking clock") {};

    void wake() override;
    void handle() override;
};

#endif // MODE_LARGETICKINGCLOCK
