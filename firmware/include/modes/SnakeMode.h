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
    enum class Stage : uint8_t // NOLINT(performance-enum-size)
    {
        READY,
        MOVE,
        DEATH,
        REMOVE,
    };

    unsigned long lastMillis{0UL};

    uint8_t blinkCount{0U};

    size_t target{0U};

    std::deque<size_t> snake{};

    std::unique_ptr<ClockHandler> clock{};

    Stage stage{0U};

    void idle();
    [[nodiscard]] std::optional<size_t> findStepPath() const;
    [[nodiscard]] std::optional<size_t> findStepAny() const;
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
