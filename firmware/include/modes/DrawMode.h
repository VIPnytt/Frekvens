#pragma once

#if MODE_DRAW

#include "config/constants.h"
#include "modules/ModeModule.h"

class DrawMode final : public ModeModule
{
private:
    bool pending = false;
    bool render = false;

    uint8_t drawing[GRID_COLUMNS * GRID_ROWS] = {0};

    void load(bool cache = false);
    void save(bool cache = false);
    void transmit();

public:
    explicit DrawMode() : ModeModule("Draw") {};

    void begin() override;
    void handle() override;
    void end() override;
    void onReceive(JsonObjectConst payload, const char *source) override;
};

#endif // MODE_DRAW
