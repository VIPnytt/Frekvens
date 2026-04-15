#pragma once

#if MODE_BLINDS

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

class BlindsMode final : public ModeModule
{
private:
    static constexpr uint8_t moduloMax = GRID_ROWS / 3;

    bool direction = true;

    uint8_t modulo = moduloMax;

    unsigned long lastMillis = 0;

public:
    static constexpr std::string_view name{"Blinds"};

    explicit BlindsMode() : ModeModule(name) {};

    void handle() override;
};

#endif // MODE_BLINDS
