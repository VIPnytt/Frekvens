#pragma once

#if MODE_BINARYCLOCK

#include "modules/ModeModule.h"

class BinaryClockMode final : public ModeModule
{
private:
    tm local = {};

    bool pending = false;

    int hour = 24;
    int minute = 60;
    int second = 60;

    void draw(uint8_t col, uint8_t value);

public:
    explicit BinaryClockMode() : ModeModule("Binary clock") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_BINARYCLOCK
