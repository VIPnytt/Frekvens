#pragma once

#if MODE_METABALLS

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <array>

class MetaballsMode final : public ModeModule
{
private:
    static constexpr float maxRadius{
        min<float>(static_cast<float>(GRID_COLUMNS * PITCH_HORIZONTAL) / static_cast<float>(PITCH_VERTICAL),
                   static_cast<float>(GRID_ROWS *PITCH_VERTICAL) / static_cast<float>(PITCH_HORIZONTAL))};
    static inline uint8_t radiusFactor{2U};
    static inline float radius{maxRadius / static_cast<float>(radiusFactor)};
    static inline float radiusSq{radius * radius};

    static constexpr float baseSpeed{1e-6F * static_cast<float>(GRID_COLUMNS * GRID_ROWS)};
    static inline uint8_t speedFactor{4U};
    static inline float speed{static_cast<float>(speedFactor) * baseSpeed};

    static constexpr uint8_t multiplier{1U << 3U};
    // How many discrete distance steps a ball's brightness falloff is quantized into, from its
    // center (0) to its edge (falloffResolution); this is the resolution of contributions below.
    static constexpr uint8_t falloffResolution{UINT8_MAX};
    // Size must stay equal to falloffResolution + 1
    std::array<uint8_t, falloffResolution + 1U> contributions{};

    struct Ball
    {
        float x;
        float y;
        float xVelocity;
        float yVelocity;
    };

    static constexpr uint8_t numBallsMax{25U};
    static inline uint8_t numBalls{(GRID_COLUMNS * GRID_ROWS / 50U)};

    std::array<Ball, numBallsMax> balls{};

    void setSpeed(uint8_t _speed);
    void setRadius(uint8_t _radius);
    void updateRadius();
    void transmit();

public:
    static constexpr std::string_view name{"Metaballs"};

    explicit MetaballsMode() : ModeModule(name) {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(JsonObjectConst payload, std::string_view source) override;
};

#endif // MODE_METABALLS
