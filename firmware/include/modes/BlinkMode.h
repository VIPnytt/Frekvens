#pragma once

#include "modules/ModeModule.h"

class BlinkMode : public ModeModule
{
private:
    unsigned long lastMillis = 0;

public:
    BlinkMode() : ModeModule("Blink") {};

    void handle() override;
};
