#pragma once

#include "modules/ModeModule.h"

class EqualizerMode : public ModeModule
{
private:
    static constexpr uint8_t width = 3;

    uint16_t interval = UINT8_MAX;

    unsigned long lastMillis = 0;

public:
    EqualizerMode() : ModeModule("Equalizer") {};

    void wake() override;
    void handle() override;
};
