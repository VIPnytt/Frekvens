#pragma once

#if MODE_GAMEOFLIFE

#include "modules/ModeModule.h"

class GameOfLifeMode : public ModeModule
{
private:
    tm local;

    bool clock = true, pending = false;

    uint8_t active = 0, hour, minute;

    unsigned long lastMillis = 0;

    void setClock(const bool _clock);
    void transmit();

public:
    GameOfLifeMode() : ModeModule("Game of Life") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(const JsonDocument doc, const char *const source) override;
};

#endif // MODE_GAMEOFLIFE
