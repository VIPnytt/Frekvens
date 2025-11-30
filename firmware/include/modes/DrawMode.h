#pragma once

#include "config/constants.h"

#if MODE_DRAW

#include "modules/ModeModule.h"

class DrawMode : public ModeModule
{
private:
    bool
        pending = false,
        render = false;

    uint8_t drawing[GRID_COLUMNS * GRID_ROWS] = {0};

    void load(const bool cache = false);
    void save(const bool cache = false);
    void transmit();

public:
    DrawMode() : ModeModule("Draw") {};

    void begin() override;
    void handle() override;
    void end() override;
    void onReceive(const JsonDocument doc, const char *const source) override;
};

#endif // MODE_DRAW
