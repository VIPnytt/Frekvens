#pragma once

#if MODE_BRIGHT

#include "modules/ModeModule.h"

class BrightMode final : public ModeModule
{
protected:
    ~BrightMode() = default;

public:
    BrightMode() : ModeModule("Bright") {};
    BrightMode(const BrightMode &) = delete;
    BrightMode &operator=(const BrightMode &) = delete;

    void begin() override;
};

#endif // MODE_BRIGHT
