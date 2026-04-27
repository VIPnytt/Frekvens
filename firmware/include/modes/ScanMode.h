#pragma once

#if MODE_SCAN

#include "modules/ModeModule.h"

class ScanMode final : public ModeModule
{
private:
    uint8_t column = 0;

    unsigned long lastMillis = 0;

public:
    static constexpr std::string_view name{"Scan"};

    explicit ScanMode() : ModeModule(name) {};

    void handle() override;
};

#endif // MODE_SCAN
