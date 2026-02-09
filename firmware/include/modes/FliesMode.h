#pragma once

#if MODE_FLIES

#include "config/constants.h"
#include "modules/ModeModule.h"

#include <unordered_map>

class FliesMode final : public ModeModule
{
private:
    struct Dot
    {
        uint8_t x = 0;
        uint8_t y = 0;
    };

    bool pending = false;

    std::unordered_map<uint8_t, Dot> flies = {};

    uint8_t drawing[GRID_COLUMNS * GRID_ROWS] = {0};

public:
    explicit FliesMode() : ModeModule("Flies") {};

    void handle() override;
    void onReceive(JsonObjectConst payload, const char *source) override;
};

#endif // MODE_FLIES
