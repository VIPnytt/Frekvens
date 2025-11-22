#pragma once

#include "config/constants.h"

#if MODE_BLINDS

#include "modules/ModeModule.h"

class BlindsMode : public ModeModule
{
protected:
    static constexpr uint8_t moduloMax = GRID_ROWS / 3;

    bool direction = true;

    uint8_t modulo = moduloMax;

    unsigned long lastMillis = 0;

public:
    BlindsMode() : ModeModule("Blinds") {};

    void handle() override;
};

#endif // MODE_BLINDS
