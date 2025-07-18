#pragma once

#include <unordered_map>
#include <vector>

#include "config/constants.h"
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

    uint8_t drawing[COLUMNS * ROWS] = {0};

public:
    FliesMode() : ModeModule("Flies") {};

    void handle() override;
    void receiverHook(const JsonDocument doc) override;
};
