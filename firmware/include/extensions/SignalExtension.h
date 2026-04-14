#pragma once

#if EXTENSION_SIGNAL

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ExtensionModule.h"

#include <array>

class SignalExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"Signal"};

    bool active = false;

    unsigned long lastMillis = 0;

    uint8_t duration = 30;

    std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> frame{};

    std::vector<std::vector<uint16_t>> signals;

    void setDuration(uint8_t seconds);
    void transmit();

public:
    explicit SignalExtension() : ExtensionModule(name) {};

    void begin() override;
    void handle() override;

    void onReceive(JsonObjectConst payload, std::string_view source) override;
};

#endif // EXTENSION_SIGNAL
