#pragma once

#if MODE_SNAKE

#include "handlers/ClockHandler.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <bits/unique_ptr.h>
#include <deque>
#include <optional>

class SnakeMode final : public ModeModule
{
private:
    struct Pixel
    {
        uint8_t x{0U}; // NOLINT(misc-non-private-member-variables-in-classes)
        uint8_t y{0U}; // NOLINT(misc-non-private-member-variables-in-classes)
        bool operator==(const Pixel &pixel) const { return x == pixel.x && y == pixel.y; }
        bool operator!=(const Pixel &pixel) const { return x != pixel.x || y != pixel.y; }
        bool operator<(const Pixel &pixel) const { return y < pixel.y || (y == pixel.y && x < pixel.x); }
    };

    unsigned long lastMillis{0UL};

    uint8_t blinkCount{0U};
    uint8_t stage{0U};

    Pixel target;

    std::deque<Pixel> snake{};

    std::unique_ptr<ClockHandler> clock{};

    void idle();
    [[nodiscard]] std::optional<Pixel> next() const;
    void move();
    void blink();
    void clean();

    void setClock(bool _clock);
    void setTarget();
    void transmit();

public:
    static constexpr std::string_view name{"Snake"};

    explicit SnakeMode() : ModeModule(name) {};

    void configure() override;
    void begin() override;
    void handle() override;

    void onReceive(JsonObjectConst payload, std::string_view source) override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif
};

#endif // MODE_SNAKE
