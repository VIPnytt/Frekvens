#pragma once

#if MODE_METABALLS

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

class MetaballsMode final : public ModeModule
{
private:
    static constexpr float radius = min<float>(GRID_COLUMNS * PITCH_HORIZONTAL / static_cast<float>(PITCH_VERTICAL),
                                               GRID_ROWS *PITCH_VERTICAL / static_cast<float>(PITCH_HORIZONTAL)) /
                                    std::numbers::pi;
    static constexpr float radiusSq = radius * radius;
    static constexpr float speed = 5e-6 * GRID_COLUMNS * GRID_ROWS;

    static constexpr uint8_t multiplier = 1U << 3U;

    struct Ball
    {
        float x;
        float y;
        float xVelocity;
        float yVelocity;
    };

    uint8_t contributions[1U << 8U]{0};

    Ball balls[GRID_COLUMNS * GRID_ROWS / (1U << 6U)]{};

public:
    explicit MetaballsMode() : ModeModule("Metaballs") {};

    void configure() override;
    void handle() override;
};

#endif // MODE_METABALLS
