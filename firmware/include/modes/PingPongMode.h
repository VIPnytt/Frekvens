#pragma once

#if MODE_PINGPONG

#include "config/constants.h"      // NOLINT(misc-include-cleaner)
#include "handlers/ClockHandler.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <bits/unique_ptr.h>
#include <deque>

class PingPongMode final : public ModeModule
{
private:
    static constexpr float speed{1e-3F * GRID_COLUMNS};

    float xDec{.0F};
    float yDec{.0F};

    std::deque<uint8_t> paddleA{};
    std::deque<uint8_t> paddleB{};

    uint8_t x{GRID_COLUMNS - 2U};
    uint8_t y{GRID_ROWS / 2U};

    uint16_t deg{135U};

    unsigned long lastMillis{0UL};

    std::unique_ptr<ClockHandler> clock{};

    void setClock(bool _clock);
    void transmit();

public:
    static constexpr std::string_view name{"Ping-pong"};

    explicit PingPongMode() : ModeModule(name) {};

    void configure() override;
    void begin() override;
    void handle() override;

    void onReceive(JsonObjectConst payload, std::string_view source) override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif
};

#endif // MODE_PINGPONG
