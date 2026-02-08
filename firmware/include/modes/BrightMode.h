#pragma once

#if MODE_BRIGHT

#include "modules/ModeModule.h"

class BrightMode final : public ModeModule
{
public:
    explicit BrightMode() : ModeModule("Bright") {};

    void begin() override;
};

#endif // MODE_BRIGHT
