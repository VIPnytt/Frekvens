#pragma once

#include "config/constants.h"

#if EXTENSION_SIGNAL

#include <ArduinoJson.h>

#include "handlers/BitmapHandler.h"
#include "modules/ExtensionModule.h"

class SignalExtension : public ExtensionModule
{
private:
    bool active = false;

    unsigned long lastMillis = 0;

    uint8_t
        duration = 30,
        frame[COLUMNS * ROWS];

    std::vector<std::vector<uint16_t>> signals;

    void setDuration(uint8_t seconds);
    void transmit();

public:
    SignalExtension();

    void ready() override;
    void handle() override;
    void receiverHook(const JsonDocument doc) override;
};

extern SignalExtension *Signal;

#endif // EXTENSION_SIGNAL
