#pragma once

#if MODE_FIREWORK

#include "config/constants.h" // NOLINT(misc-include-cleaner)
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
    static constexpr std::string_view name{"Firework"};

    explicit FireworkMode() : ModeModule(name) {};

    void handle() override;
};

#endif // MODE_FIREWORK
