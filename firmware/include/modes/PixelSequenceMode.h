#pragma once

#if MODE_PIXELSEQUENCE

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <array>

class PixelSequenceMode final : public ModeModule
{
private:
#if GRID_COLUMNS * GRID_ROWS <= 0b1U << 8U
    static inline std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> pixels{};
#else
    static inline std::array<uint16_t, GRID_COLUMNS * GRID_ROWS> pixels{};
#endif // GRID_COLUMNS * GRID_ROWS <= 0b1U << 8U

    bool lit{true};

    uint16_t idx{0U};

    unsigned long lastMillis{0UL};

public:
    static constexpr std::string_view name{"Pixel sequence"};

    explicit PixelSequenceMode() : ModeModule(name) {};

    void configure() override;
    void handle() override;
};

#endif // MODE_PIXELSEQUENCE
