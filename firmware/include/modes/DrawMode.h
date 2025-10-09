#pragma once

#include "config/constants.h"

#if MODE_DRAW

#include "modules/ModeModule.h"

class DrawMode : public ModeModule
{
private:
    bool pending = false;

    uint8_t drawing[COLUMNS * ROWS] = {0};

    void load(bool cache = false);
    void save(bool cache = false);
    void transmit();

public:
    DrawMode() : ModeModule("Draw") {};

    void setup() override;
    void wake() override;
    void handle() override;
    void sleep() override;
    void receiverHook(const JsonDocument doc) override;
};

#endif // MODE_DRAW
