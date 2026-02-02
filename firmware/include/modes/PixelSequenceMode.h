#pragma once

#if MODE_PIXELSEQUENCE

#include "config/constants.h"
#include "modules/ModeModule.h"

class PixelSequenceMode final : public ModeModule
{
private:
    static constexpr uint8_t pixelOrder[GRID_COLUMNS * GRID_ROWS] = LED_MAP;

    bool lit = true;

    uint16_t address = 0;

    unsigned long lastMillis = 0;

protected:
    ~PixelSequenceMode() = default;

public:
    PixelSequenceMode() : ModeModule("Pixel sequence") {};
    PixelSequenceMode(const PixelSequenceMode &) = delete;
    PixelSequenceMode &operator=(const PixelSequenceMode &) = delete;

    void handle() override;
};

#endif // MODE_PIXELSEQUENCE
