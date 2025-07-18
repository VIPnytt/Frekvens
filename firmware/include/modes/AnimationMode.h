#pragma once

#include <vector>

#include "modules/ModeModule.h"

class AnimationMode : public ModeModule
{
private:
    uint8_t index = 0;

    uint16_t duration = 500;

    unsigned long lastMillis = 0;

    void transmit();

public:
    AnimationMode() : ModeModule("Animation") {};

    void setup() override;
    void handle() override;
    void receiverHook(const JsonDocument doc) override;
};
