#pragma once

#if MODE_ANIMATION

#include "config/constants.h"
#include "modules/ModeModule.h"

#include <vector>

class AnimationMode final : public ModeModule
{
private:
    bool pending = false;

    uint8_t index = 0;

    uint16_t interval = 500;

    unsigned long lastMillis = 0;

    void setFrame(uint8_t index, const uint8_t frame[GRID_COLUMNS * GRID_ROWS]);
    void setFrames(uint8_t count);
    void setInterval(uint16_t interval);

    void transmit(uint8_t index, const uint8_t frame[GRID_COLUMNS * GRID_ROWS]);

public:
    explicit AnimationMode() : ModeModule("Animation") {};

    void begin() override;
    void handle() override;

    void onReceive(JsonObjectConst payload, const char *source) override;
};

#endif // MODE_ANIMATION
