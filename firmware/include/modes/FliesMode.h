#pragma once

#if MODE_FLIES

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <unordered_map>

class FliesMode final : public ModeModule
{
private:
    bool pending{false};

    std::unordered_map<uint8_t, std::pair<uint8_t, uint8_t>> flies{};

public:
    static constexpr std::string_view name{"Flies"};

    explicit FliesMode() : ModeModule(name) {};

    void handle() override;
    void onReceive(JsonObjectConst payload, std::string_view source) override;
};

#endif // MODE_FLIES
