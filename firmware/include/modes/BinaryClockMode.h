#pragma once

#include "modules/ModeModule.h"

class BinaryClockMode : public ModeModule
{
private:
    bool pending = true;

    tm local;

    uint8_t
        hour,
        min,
        sec;

    void draw(uint8_t col, uint8_t value);

public:
    BinaryClockMode() : ModeModule("Binary clock") {};

    void wake() override;
    void handle() override;
};
