#pragma once

#if MODE_FLIES

#include "config/constants.h"
#include "modules/ModeModule.h"

#include <unordered_map>

class FliesMode : public ModeModule
{
private:
    struct Dot
    {
        uint8_t
            x,
            y;
    };

    bool pending = false;

    std::unordered_map<uint8_t, Dot> flies;

    uint8_t drawing[GRID_COLUMNS * GRID_ROWS] = {0};

public:
    FliesMode() : ModeModule("Flies") {};

    void handle() override;
    void onReceive(const JsonDocument doc, const char *const source) override;
};

#endif // MODE_FLIES
