#pragma once

#if MODE_ANIMATION

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <span>

class AnimationMode final : public ModeModule
{
private:
    bool pending = false;

    uint8_t index = 0;

    uint16_t interval = 500;

    unsigned long lastMillis = 0;

    void setFrame(uint8_t _index, std::span<const uint8_t> frame);
    void setFrames(uint8_t count);
    void setInterval(uint16_t _interval);

    void transmit(uint8_t index, std::span<const uint8_t> frame);

public:
    explicit AnimationMode() : ModeModule("Animation") {};

    void begin() override;
    void handle() override;

    void onReceive(JsonObjectConst payload, std::string_view source) override;
};

#endif // MODE_ANIMATION
