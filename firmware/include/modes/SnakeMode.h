#pragma once

#if MODE_SNAKE

#include "modules/ModeModule.h"

#include <deque>
#include <optional>

class SnakeMode final : public ModeModule
{
private:
    struct Pixel
    {
        uint8_t x = 0;
        uint8_t y = 0;
        bool operator==(const Pixel &pixel) const { return x == pixel.x && y == pixel.y; }
        bool operator!=(const Pixel &pixel) const { return x != pixel.x || y != pixel.y; }
        bool operator<(const Pixel &pixel) const { return y < pixel.y || (y == pixel.y && x < pixel.x); }
    };

    tm local = {};

    bool clock = true;
    bool pending = false;

    unsigned long lastMillis = 0;

    int hour = 24;
    int minute = 60;

    uint8_t n = 0;
    uint8_t stage = 0;

    Pixel dot;

    std::deque<Pixel> snake = {};

    void idle();
    [[nodiscard]] std::optional<Pixel> next() const;
    void move();
    void blink();
    void clean();

    void setClock(bool _clock);
    void setDot();
    void transmit();

public:
    explicit SnakeMode() : ModeModule("Snake") {}

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(JsonObjectConst payload, const char *source) override;
};

#endif // MODE_SNAKE
