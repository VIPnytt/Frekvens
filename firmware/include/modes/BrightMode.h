#pragma once

#include "config/constants.h"

#if MODE_BRIGHT

#include "modules/ModeModule.h"

class BrightMode : public ModeModule
{
public:
    BrightMode() : ModeModule("Bright") {};

    void wake() override;
};

#endif // MODE_BRIGHT
