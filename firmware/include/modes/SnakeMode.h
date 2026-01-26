#pragma once

#if MODE_SNAKE

#include "modules/ModeModule.h"

#include <deque>

class SnakeMode : public ModeModule
{
private:
    struct Pixel
    {
        uint8_t
            x,
            y;

        bool operator==(const Pixel &pixel) const
        {
            return x == pixel.x && y == pixel.y;
        }
        bool operator<(const Pixel &pixel) const
        {
            return (y < pixel.y) || (y == pixel.y && x < pixel.x);
        }
    };

    unsigned long lastMillis = 0;

    uint8_t
        blink = 0,
        stage = 0;

    std::deque<Pixel> snake;

    Pixel dot;

    void idle();
    void move();
    void end();
    void clean();

    void setDot();
    bool findPath(Pixel start, Pixel goal, Pixel &next);

public:
    SnakeMode() : ModeModule("Snake") {}

    void begin() override;
    void handle() override;
};

#endif // MODE_SNAKE
