#pragma once

#if MODE_PIXELSEQUENCE

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <array>

class PixelSequenceMode final : public ModeModule
{
private:
    static constexpr std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> pixelOrder{LED_MAP};

    bool lit = true;

    uint16_t address = 0;

    unsigned long lastMillis = 0;

public:
    static constexpr std::string_view name{"Pixel sequence"};

    explicit PixelSequenceMode() : ModeModule(name) {};

    void handle() override;
};

#endif // MODE_PIXELSEQUENCE
