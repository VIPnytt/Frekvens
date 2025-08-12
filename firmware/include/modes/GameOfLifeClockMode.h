#pragma once

#include "modules/ModeModule.h"

class GameOfLifeClockMode : public ModeModule
{
private:
    tm local;

    bool pending = false;

    uint8_t
        active,
        hour,
        minute;

    unsigned long lastMillis = 0;

public:
    GameOfLifeClockMode() : ModeModule("Game of Life clock") {};

    void wake() override;
    void handle() override;
};
