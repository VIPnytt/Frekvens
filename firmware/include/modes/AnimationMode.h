#pragma once

#include "config/constants.h"

#if MODE_ANIMATION

#include <vector>

#include "modules/ModeModule.h"

class AnimationMode : public ModeModule
{
private:
    bool pending = false;

    uint8_t index = 0;

    uint16_t duration = 500;

    unsigned long lastMillis = 0;

    void transmit(const uint8_t index, const uint8_t frame[GRID_COLUMNS * GRID_ROWS]);

public:
    AnimationMode() : ModeModule("Animation") {};

    void wake() override;
    void handle() override;
    void receiverHook(const JsonDocument doc, const char *const source) override;
};

#endif // MODE_ANIMATION
