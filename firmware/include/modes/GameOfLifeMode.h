#pragma once

#if MODE_GAMEOFLIFE

#include "modules/ModeModule.h"

class GameOfLifeMode : public ModeModule
{
private:
    tm local;

    bool clock = true;
    bool pending = false;

    uint8_t active = 0;
    uint8_t hour = 24;
    uint8_t minute = 60;

    unsigned long lastMillis = 0;

    void setClock(bool _clock);
    void transmit();

public:
    GameOfLifeMode() : ModeModule("Game of Life") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(const JsonDocument &doc, const char *source) override;
};

#endif // MODE_GAMEOFLIFE
