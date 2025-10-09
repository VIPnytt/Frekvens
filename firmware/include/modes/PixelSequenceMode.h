#pragma once

#include "config/constants.h"

#if MODE_PIXELSEQUENCE

#include "modules/ModeModule.h"

class PixelSequenceMode : public ModeModule
{
private:
    static constexpr uint8_t pixelOrder[COLUMNS * ROWS] = PIXEL_ORDER;

    uint8_t brightness = UINT8_MAX;

    uint16_t address = 0;

    unsigned long lastMillis = 0;

public:
    PixelSequenceMode() : ModeModule("Pixel sequence") {};

    void handle() override;
};

#endif // MODE_PIXELSEQUENCE
