#pragma once

#if MODE_SCAN

#include "modules/ModeModule.h"

class ScanMode final : public ModeModule
{
private:
    uint8_t column = 0;

    unsigned long lastMillis = 0;

protected:
    ~ScanMode() = default;

public:
    ScanMode() : ModeModule("Scan") {};
    ScanMode(const ScanMode &) = delete;
    ScanMode &operator=(const ScanMode &) = delete;

    void handle() override;
};

#endif // MODE_SCAN
