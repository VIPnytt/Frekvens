#pragma once

#if MODE_SNAKE

#include "handlers/ClockHandler.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <array>
#include <bits/unique_ptr.h>
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

    size_t head{0U};
    size_t length{0U};
    size_t target{0U};

    std::array<size_t, GRID_COLUMNS * GRID_ROWS> snake{};
    std::array<bool, GRID_COLUMNS * GRID_ROWS> occupied{};

    std::unique_ptr<ClockHandler> clock{};

    Stage stage{Stage::READY};

    void blink();
    void clean();
    void idle();
    void move();
    void setClock(bool _clock);
    void setDead();
    void setTarget();
    void snakeReset(size_t start);
    void snakeClear();
    void transmit();

    [[nodiscard]] bool snakePushBack(size_t pixel);

    [[nodiscard]] size_t snakeAt(size_t index) const;
    [[nodiscard]] size_t snakePopFront();

    [[nodiscard]] std::optional<size_t> findStepAvailable() const;
    [[nodiscard]] std::optional<size_t> findStepPath() const;

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
