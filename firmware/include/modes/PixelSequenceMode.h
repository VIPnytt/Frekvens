#pragma once

#if MODE_PIXELSEQUENCE

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <array>

class PixelSequenceMode final : public ModeModule
{
private:
    static inline std::array<size_t, GRID_COLUMNS * GRID_ROWS> pixels{};

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
