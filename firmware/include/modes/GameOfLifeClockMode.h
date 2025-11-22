#pragma once

#include "config/constants.h"

#if MODE_GAMEOFLIFECLOCK

#include "modules/ModeModule.h"

class GameOfLifeClockMode : public ModeModule
{
private:
    tm local;

    bool pending = false;

    uint8_t
        active = 0,
        hour,
        minute;

    unsigned long lastMillis = 0;

public:
    GameOfLifeClockMode() : ModeModule("Game of Life clock") {};

    void wake() override;
    void handle() override;
};

#endif // MODE_GAMEOFLIFECLOCK
