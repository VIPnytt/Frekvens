#pragma once

#include "modules/ModeModule.h"

class TickingClockMode : public ModeModule
{
private:
    tm local;

    uint8_t
        hour,
        minute,
        secound = 0;

public:
    TickingClockMode() : ModeModule("Ticking clock") {};

    void handle() override;
};
