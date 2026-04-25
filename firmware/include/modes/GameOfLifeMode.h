#pragma once

#if MODE_GAMEOFLIFE

#include "modules/ModeModule.h"

class GameOfLifeMode final : public ModeModule
{
private:
    static inline bool clock = true;

    tm local{};

    bool pending = false;

    uint8_t active = 0;

    int hour = 24;
    int minute = 60;

    unsigned long lastMillis = 0;

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
