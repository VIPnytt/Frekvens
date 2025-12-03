#pragma once

#if MODE_SCAN

#include "modules/ModeModule.h"

class ScanMode : public ModeModule
{
private:
    uint8_t column = 0;

    unsigned long lastMillis = 0;

public:
    ScanMode() : ModeModule("Scan") {};

    void handle() override;
};

#endif // MODE_SCAN
