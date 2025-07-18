#pragma once

#include "modules/ModeModule.h"

class SmallClockMode : public ModeModule
{
private:
    tm local;

    uint8_t
        hour,
        min;

public:
    SmallClockMode() : ModeModule("Small clock") {};

    void handle() override;
};
