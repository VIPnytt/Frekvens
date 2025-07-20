#pragma once

#include "modules/ModeModule.h"

class BoldClockMode : public ModeModule
{
private:
    tm local;

    uint8_t
        hour,
        minute;

public:
    BoldClockMode() : ModeModule("Bold clock") {};

    void handle() override;
};
