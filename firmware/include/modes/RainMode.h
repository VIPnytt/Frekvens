#pragma once

#if MODE_RAIN

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <array>

class RainMode final : public ModeModule
{
private:
    struct Drop
    {
        uint8_t delay{INT8_MAX};
        uint8_t length{3U};
        uint8_t x{0U};
        uint8_t y{0U};
        unsigned long lastMillis{0UL};
    };

    std::array<Drop, GRID_COLUMNS / 3U> drops{};

public:
    static constexpr std::string_view name{"Rain"};

    explicit RainMode() : ModeModule(name) {};

    void begin() override;
    void handle() override;
};

#endif // MODE_RAIN
