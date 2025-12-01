#pragma once

#if MODE_EQUALIZER

#include "config/constants.h"
#include "modules/ModeModule.h"

class EqualizerMode : public ModeModule
{
private:
    static constexpr uint8_t width = 3;

    struct Bar
    {
        uint8_t
            level = GRID_ROWS - 1,
            target = GRID_ROWS - 1;
    };

    Bar bars[GRID_COLUMNS / (width + 1)];

    unsigned long lastMillis = 0;

public:
    EqualizerMode() : ModeModule("Equalizer") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_EQUALIZER
