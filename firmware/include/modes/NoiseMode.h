#pragma once

#if MODE_NOISE

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <array>

class NoiseMode final : public ModeModule
{
private:
    struct Dot
    {
        uint8_t x{0U};
        uint8_t y{0U};
        uint8_t delay{0U};
        unsigned long lastMillis{0UL};
    };

    std::array<Dot, GRID_COLUMNS * GRID_ROWS / 9U> dots{};

public:
    static constexpr std::string_view name{"Noise"};

    explicit NoiseMode() : ModeModule(name) {};

    void handle() override;
};

#endif // MODE_NOISE
