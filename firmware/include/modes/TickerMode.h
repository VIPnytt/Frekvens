#pragma once

#include <bits/unique_ptr.h>

#include "handlers/TextHandler.h"
#include "modules/ModeModule.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

class TickerMode : public ModeModule
{
private:
    bool pending = false;

    int8_t offsetY;

    int16_t
        offsetX,
        textWidth;

    unsigned long lastMillis = 0;

    FontModule *font = nullptr;

    String message;

    std::unique_ptr<TextHandler> text;

    void setFont(const char *const fontName);
    void setMessage(String textMessage);

    void transmit();

public:
    TickerMode() : ModeModule("Ticker") {};

    void setup() override;
    void wake() override;
    void handle() override;
    void sleep() override;

    void receiverHook(const JsonDocument doc) override;
};
