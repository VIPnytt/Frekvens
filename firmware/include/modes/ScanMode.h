#pragma once

#include "config/constants.h"

#if MODE_SCAN

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

#endif // MODE_SCAN
