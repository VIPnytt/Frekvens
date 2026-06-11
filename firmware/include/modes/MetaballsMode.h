#pragma once

#if MODE_METABALLS

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <array>

class MetaballsMode final : public ModeModule
{
private:
    static constexpr float radius{min<float>(GRID_COLUMNS * PITCH_HORIZONTAL / static_cast<float>(PITCH_VERTICAL),
                                             GRID_ROWS *PITCH_VERTICAL / static_cast<float>(PITCH_HORIZONTAL)) /
                                  5.0F};
    static constexpr float radiusSq{radius * radius};
    static constexpr float speed{1e-6F * static_cast<float>(GRID_COLUMNS * GRID_ROWS)};

    static constexpr uint8_t multiplier{1U << 4U};

    struct Ball
    {
        float x;
        float y;
        float xVelocity;
        float yVelocity;
    };

    std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> contributions{};
    std::array<Ball, GRID_COLUMNS * GRID_ROWS / 50U> balls{};

public:
    static constexpr std::string_view name{"Metaballs"};

    explicit MetaballsMode() : ModeModule(name) {};

    void begin() override;
    void handle() override;
};

#endif // MODE_METABALLS
