#pragma once

#include "modules/ModeModule.h"

class SmallClockMode : public ModeModule
{
private:
    tm local;

    uint8_t
        hour,
        minute;

public:
    SmallClockMode() : ModeModule("Small clock") {};

    void handle() override;
};
