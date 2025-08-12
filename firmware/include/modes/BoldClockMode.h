#pragma once

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

    void wake() override;
    void handle() override;
};
