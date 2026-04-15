#pragma once

#if MODE_DRAW

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <array>

class DrawMode final : public ModeModule
{
private:
    bool pending = false;
    bool render = false;

    std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> frame{};

    void load(bool cache = false);
    void save(bool cache = false);
    void transmit();

public:
    static constexpr std::string_view name{"Draw"};

    explicit DrawMode() : ModeModule(name) {};

    void begin() override;
    void handle() override;
    void end() override;
    void onReceive(JsonObjectConst payload, std::string_view source) override;
};

#endif // MODE_DRAW
