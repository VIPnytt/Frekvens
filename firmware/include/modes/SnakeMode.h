#pragma once

#if MODE_SNAKE

#include "modules/ModeModule.h"

#include <deque>

class SnakeMode : public ModeModule
{
private:
    struct Pixel
    {
        uint8_t x, y;

        bool operator==(const Pixel &pixel) const { return x == pixel.x && y == pixel.y; }

        bool operator<(const Pixel &pixel) const { return (y < pixel.y) || (y == pixel.y && x < pixel.x); }
    };

    tm local;

    bool clock = true, pending = false;

    unsigned long lastMillis = 0;

    uint8_t blink = 0, hour, minute, stage = 0;

    std::deque<Pixel> snake;

    Pixel dot;

    void idle();
    void move();
    void end();
    void clean();

    bool findPath(Pixel start, Pixel goal, Pixel &next);

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
