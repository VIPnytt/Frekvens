#pragma once

#if MODE_BLINDS

#include "config/constants.h"
#include "modules/ModeModule.h"

class BlindsMode final : public ModeModule
{
private:
    static constexpr uint8_t moduloMax = GRID_ROWS / 3;

    bool direction = true;

    uint8_t modulo = moduloMax;

    unsigned long lastMillis = 0;

protected:
    ~BlindsMode() = default;

public:
    BlindsMode() : ModeModule("Blinds") {};
    BlindsMode(const BlindsMode &) = delete;
    BlindsMode &operator=(const BlindsMode &) = delete;

    void handle() override;
};

#endif // MODE_BLINDS
