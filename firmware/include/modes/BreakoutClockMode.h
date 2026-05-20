#pragma once

#if MODE_BREAKOUTCLOCK

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <deque>

class BreakoutClockMode final : public ModeModule
{
private:
    static constexpr float speed{7e-4F * GRID_ROWS};

    float xDec{GRID_COLUMNS / 2.0F};
    float yDec{GRID_ROWS - 2.0F};

    uint8_t x{GRID_COLUMNS / 2U};
    uint8_t y{GRID_ROWS - 2U};

    uint16_t deg{90U};

    int hour{24};
    int minute{60};

    std::deque<uint8_t> paddle{};

    tm local{};

public:
    static constexpr std::string_view name{"Breakout clock"};

    explicit BreakoutClockMode() : ModeModule(name) {};

    void begin() override;
    void handle() override;
};

#endif // MODE_BREAKOUTCLOCK
