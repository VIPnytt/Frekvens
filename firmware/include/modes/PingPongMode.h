#pragma once

#if MODE_PINGPONG

#include "config/constants.h"
#include "modules/ModeModule.h"

#include <deque>

class PingPongMode : public ModeModule
{
private:
    static constexpr float speed = 1e-3 * GRID_COLUMNS;

    bool clock = true, pending = false;

    float xDec, yDec;

    std::deque<uint8_t> paddleA, paddleB;

    tm local;

    uint8_t hour, minute, x = GRID_COLUMNS - 2, y = GRID_ROWS / 2;

    uint16_t deg = 135;

    unsigned long lastMillis = 0;

    void setClock(const bool _clock);
    void transmit();

public:
    PingPongMode() : ModeModule("Ping-pong") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(const JsonDocument doc, const char *const source) override;
};

#endif // MODE_PINGPONG
