#pragma once

#include "modules/ModeModule.h"

class ScanMode : public ModeModule
{
private:
    uint8_t x = 0;

    unsigned long lastMillis = 0;

public:
    ScanMode() : ModeModule("Scan") {};

    void handle() override;
};
