#pragma once

#include "config/constants.h"

#if MODE_BOLDCLOCK

#include "modules/ModeModule.h"

class BoldClockMode : public ModeModule
{
private:
    tm local;

    bool pending = false;

    uint8_t
        hour,
        minute;

public:
    BoldClockMode() : ModeModule("Bold clock") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_BOLDCLOCK
