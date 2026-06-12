#pragma once

#if MODE_LEAFFALL

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <array>

class LeafFallMode final : public ModeModule
{
private:
    struct Leaf
    {
        uint8_t x{0U};
        uint8_t y{0U};
        uint8_t brightness{INT8_MAX};
        uint16_t delay{UINT8_MAX};
        unsigned long lastMillis{0UL};
    };

    std::array<Leaf, GRID_COLUMNS * GRID_ROWS / 20U> leaves{};

public:
    static constexpr std::string_view name{"Leaf fall"};

    explicit LeafFallMode() : ModeModule(name) {};

    void begin() override;
    void handle() override;
};

#endif // MODE_LEAFFALL
