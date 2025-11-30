#pragma once

#include "config/constants.h"

#if MODE_BINARYCLOCK

#include "modules/ModeModule.h"

class BinaryClockMode : public ModeModule
{
private:
    tm local;

    bool pending = false;

    uint8_t
        hour,
        minute,
        second;

    void draw(uint8_t col, uint8_t value);

public:
    BinaryClockMode() : ModeModule("Binary clock") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_BINARYCLOCK
