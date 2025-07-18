#pragma once

#include "modules/ModeModule.h"

class BlindsMode : public ModeModule
{
protected:
    static constexpr uint8_t moduloMax = ROWS / 3;

    bool direction = true;

    uint8_t modulo = moduloMax;

    unsigned long lastMillis = 0;

public:
    BlindsMode() : ModeModule("Blinds") {};

    void handle() override;
};
