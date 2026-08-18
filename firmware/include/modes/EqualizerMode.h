#pragma once

#if MODE_EQUALIZER

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <array>

class EqualizerMode final : public ModeModule
{
private:
    static constexpr uint8_t width{3U};

    std::array<std::pair<uint8_t, uint8_t>, GRID_COLUMNS / (width + 1U)> bars{};

    /**
 * Initializes the Equalizer mode.
 */
unsigned long lastMillis{0UL};

public:
    static constexpr std::string_view name{"Equalizer"};

    explicit EqualizerMode() : ModeModule(name) {};

    void begin() override;
    void handle() override;
};

#endif // MODE_EQUALIZER
