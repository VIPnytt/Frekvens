#pragma once

#if MODE_EQUALIZER

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <array>

class EqualizerMode final : public ModeModule
{
private:
    static constexpr uint8_t width{3U};

    struct Bar
    {
        uint8_t level{GRID_ROWS - 1U};
        uint8_t target{GRID_ROWS - 1U};
    };

    std::array<Bar, GRID_COLUMNS / (width + 1U)> bars{};

    unsigned long lastMillis{0UL};

public:
    static constexpr std::string_view name{"Equalizer"};

    explicit EqualizerMode() : ModeModule(name) {};

    void begin() override;
    void handle() override;
};

#endif // MODE_EQUALIZER
