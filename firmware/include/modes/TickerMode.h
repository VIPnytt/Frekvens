#pragma once

#if MODE_TICKER

#include "config/constants.h"
#include "handlers/TextHandler.h"
#include "modules/ModeModule.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

#include <bits/unique_ptr.h>

class TickerMode final : public ModeModule
{
private:
    bool pending = false;

    int8_t offsetY = GRID_ROWS / 2;

    int16_t offsetX = GRID_COLUMNS;
    int16_t width = 0;

    unsigned long lastMillis = 0;

    FontModule *font = nullptr;

    std::string message = NAME;

    std::unique_ptr<TextHandler> text = nullptr;

    void setFont(const char *fontName);
    void setMessage(std::string _message);

    void transmit();

public:
    explicit TickerMode() : ModeModule("Ticker") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void end() override;

    void onReceive(JsonObjectConst payload, const char *source) override;
};

#endif // MODE_TICKER
