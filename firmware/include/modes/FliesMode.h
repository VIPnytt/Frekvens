#pragma once

#if MODE_FLIES

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <unordered_map>

class FliesMode final : public ModeModule
{
private:
    struct Dot
    {
        uint8_t x = 0;
        uint8_t y = 0;
    };

    bool pending = false;

    std::unordered_map<uint8_t, Dot> flies{};

public:
    static constexpr std::string_view name{"Flies"};

    explicit FliesMode() : ModeModule(name) {};

    void handle() override;
    void onReceive(JsonObjectConst payload, std::string_view source) override;
};

#endif // MODE_FLIES
