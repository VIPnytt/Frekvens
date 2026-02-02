#pragma once

#if MODE_BINARYCLOCK

#include "modules/ModeModule.h"

class BinaryClockMode : public ModeModule
{
private:
    tm local;

    bool pending = false;

    uint8_t hour = 24;
    uint8_t minute = 60;
    uint8_t second = 60;

    void draw(uint8_t col, uint8_t value);

public:
    BinaryClockMode() : ModeModule("Binary clock") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_BINARYCLOCK
