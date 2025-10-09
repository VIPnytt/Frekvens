#pragma once

#include "config/constants.h"

#if MODE_SNAKECLOCK

#include <deque>

#include "modules/ModeModule.h"

class SnakeClockMode : public ModeModule
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

    tm local;

    bool pending = false;

    unsigned long lastMillis = 0;

    uint8_t
        blink = 0,
        hour,
        minute,
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
    SnakeClockMode() : ModeModule("Snake clock") {}

    void wake() override;
    void handle() override;
};

#endif // MODE_SNAKECLOCK
