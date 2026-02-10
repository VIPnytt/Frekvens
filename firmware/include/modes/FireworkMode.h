#pragma once

#if MODE_FIREWORK

#include "config/constants.h"
#include "modules/ModeModule.h"

class FireworkMode final : public ModeModule
{
private:
    unsigned long lastMillis = 0;

    uint8_t brightness = UINT8_MAX;
    uint8_t maxRadius = 1;
    uint8_t radius = 0;
    uint8_t rocketX = 0;
    uint8_t rocketY = GRID_ROWS;
    uint8_t stage = 0;

    void pad();
    void launching();
    void exploding();
    void fading();

public:
    explicit FireworkMode() : ModeModule("Firework") {};

    void handle() override;
};

#endif // MODE_FIREWORK
