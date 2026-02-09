#pragma once

#if MODE_GAMEOFLIFE

#include "modules/ModeModule.h"

class GameOfLifeMode final : public ModeModule
{
private:
    tm local = {};

    bool clock = true;
    bool pending = false;

    uint8_t active = 0;
    int hour = 24;
    int minute = 60;

    unsigned long lastMillis = 0;

    void setClock(bool _clock);
    void transmit();

public:
    explicit GameOfLifeMode() : ModeModule("Game of Life") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(JsonObjectConst payload, const char *source) override;
};

#endif // MODE_GAMEOFLIFE
