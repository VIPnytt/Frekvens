#pragma once

#include "config/constants.h"

#if MODE_FLIES

#include <unordered_map>
#include <vector>

#include "modules/ModeModule.h"

class FliesMode : public ModeModule
{
private:
    struct Pixel
    {
        uint8_t
            x,
            y;
    };

    bool pending = false;

    std::unordered_map<uint8_t, Pixel> flies;

    uint8_t drawing[GRID_COLUMNS * GRID_ROWS] = {0};

public:
    FliesMode() : ModeModule("Flies") {};

    void handle() override;
    void receiverHook(const JsonDocument doc, const char *const source) override;
};

#endif // MODE_FLIES
