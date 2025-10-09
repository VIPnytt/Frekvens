#pragma once

#include "config/constants.h"

#if MODE_BLINK

#include "modules/ModeModule.h"

class BlinkMode : public ModeModule
{
private:
    unsigned long lastMillis = 0;

public:
    BlinkMode() : ModeModule("Blink") {};

    void handle() override;
};

#endif // MODE_BLINK
