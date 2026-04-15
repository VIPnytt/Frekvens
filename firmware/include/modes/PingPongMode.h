#pragma once

#if MODE_PINGPONG

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <deque>

class PingPongMode final : public ModeModule
{
private:
    static constexpr float speed = 1e-3 * GRID_COLUMNS;

    bool clock = true;
    bool pending = false;

    float xDec = .0F;
    float yDec = .0F;

    std::deque<uint8_t> paddleA{};
    std::deque<uint8_t> paddleB{};

    tm local{};

    int hour = 24;
    int minute = 60;

    uint8_t x = GRID_COLUMNS - 2;
    uint8_t y = GRID_ROWS / 2;

    uint16_t deg = 135;

    unsigned long lastMillis = 0;

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
