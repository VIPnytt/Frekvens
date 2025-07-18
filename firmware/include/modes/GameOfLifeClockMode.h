#pragma once

#include "modules/ModeModule.h"

class GameOfLifeClockMode : public ModeModule
{
private:
    tm local;

    uint8_t
        active,
        hour,
        min;

    unsigned long lastMillis = 0;

public:
    GameOfLifeClockMode() : ModeModule("Game of Life clock") {};

    void wake() override;
    void handle() override;
};
