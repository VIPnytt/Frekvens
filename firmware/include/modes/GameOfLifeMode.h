#pragma once

#if MODE_GAMEOFLIFE

#include "handlers/ClockHandler.h"
#include "modules/ModeModule.h"

#include <bits/unique_ptr.h>

class GameOfLifeMode final : public ModeModule
{
private:
    uint8_t active{0U};
    uint8_t brightness{INT8_MAX};
    uint8_t yMin{0U};

    unsigned long lastMillis{0UL};

    std::unique_ptr<ClockHandler> clock{};

    void setClock(bool _clock);
    void transmit();

public:
    static constexpr std::string_view name{"Game of Life"};

    explicit GameOfLifeMode() : ModeModule(name) {};

    void configure() override;
    void begin() override;
    void handle() override;

    void onReceive(JsonObjectConst payload, std::string_view source) override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif
};

#endif // MODE_GAMEOFLIFE
