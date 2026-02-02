#pragma once

#if MODE_BLINK

#include "modules/ModeModule.h"

class BlinkMode final : public ModeModule
{
private:
    unsigned long lastMillis = 0;

protected:
    ~BlinkMode() = default;

public:
    BlinkMode() : ModeModule("Blink") {};
    BlinkMode(const BlinkMode &) = delete;
    BlinkMode &operator=(const BlinkMode &) = delete;

    void handle() override;
};

#endif // MODE_BLINK
