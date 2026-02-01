#pragma once

#if MODE_SNAKE

#include "modules/ModeModule.h"

#include <deque>
#include <optional>

class SnakeMode : public ModeModule
{
private:
    struct Pixel
    {
        uint8_t x;
        uint8_t y;

        bool operator==(const Pixel &pixel) const { return x == pixel.x && y == pixel.y; }
        bool operator!=(const Pixel &pixel) const { return x != pixel.x || y != pixel.y; }
        bool operator<(const Pixel &pixel) const { return (y < pixel.y) || (y == pixel.y && x < pixel.x); }
    };

    tm local;

    bool clock = true;
    bool pending = false;

    unsigned long lastMillis = 0;

    uint8_t blink = 0;
    uint8_t hour;
    uint8_t minute;
    uint8_t stage = 0;

    Pixel dot;

    std::deque<Pixel> snake;

    std::optional<Pixel> next();

    void idle();
    void move();
    void end();
    void clean();

    void setClock(const bool _clock);
    void setDot();
    void transmit();

public:
    SnakeMode() : ModeModule("Snake") {}

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(const JsonDocument doc, const char *const source) override;
};

#endif // MODE_SNAKE
