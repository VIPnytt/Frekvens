#pragma once

#if MODE_GAMEOFLIFE

#include "modules/ModeModule.h"

class GameOfLifeMode : public ModeModule
{
private:
    unsigned long lastMillis = 0;

    uint8_t active = 0;

public:
    GameOfLifeMode() : ModeModule("Game of Life") {};

    void handle() override;
};

#endif // MODE_GAMEOFLIFE
