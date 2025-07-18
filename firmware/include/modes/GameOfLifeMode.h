#pragma once

#include "modules/ModeModule.h"

class GameOfLifeMode : public ModeModule
{
private:
    unsigned long lastMillis = 0;

    uint8_t active;

public:
    GameOfLifeMode() : ModeModule("Game of Life") {};

    void handle() override;
};
