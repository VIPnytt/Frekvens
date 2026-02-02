#pragma once

#if MODE_PINGPONG

#include "config/constants.h"
#include "modules/ModeModule.h"

#include <deque>

class PingPongMode final : public ModeModule
{
private:
    static constexpr float speed = 1e-3 * GRID_COLUMNS;

    bool clock = true;
    bool pending = false;

    float xDec;
    float yDec;

    std::deque<uint8_t> paddleA;
    std::deque<uint8_t> paddleB;

    tm local = {};

    int hour = 24;
    int minute = 60;

    uint8_t x = GRID_COLUMNS - 2;
    uint8_t y = GRID_ROWS / 2;

    uint16_t deg = 135;

    unsigned long lastMillis = 0;

    void setClock(bool _clock);
    void transmit();

protected:
    ~PingPongMode() = default;

public:
    PingPongMode() : ModeModule("Ping-pong") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(const JsonDocument &doc, const char *source) override;
};

#endif // MODE_PINGPONG
