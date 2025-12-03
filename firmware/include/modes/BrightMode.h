#pragma once

#if MODE_BRIGHT

#include "modules/ModeModule.h"

class BrightMode : public ModeModule
{
public:
    BrightMode() : ModeModule("Bright") {};

    void begin() override;
};

#endif // MODE_BRIGHT
