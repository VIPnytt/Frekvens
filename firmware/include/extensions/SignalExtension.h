#pragma once

#if EXTENSION_SIGNAL

#include "config/constants.h"
#include "handlers/BitmapHandler.h"
#include "modules/ExtensionModule.h"

class SignalExtension : public ExtensionModule
{
private:
    bool active = false;

    unsigned long lastMillis = 0;

    uint8_t
        duration = 30,
        frame[GRID_COLUMNS * GRID_ROWS] = {0};

    std::vector<std::vector<uint16_t>> signals;

    void setDuration(uint8_t seconds);
    void transmit();

public:
    SignalExtension();

    void begin() override;
    void handle() override;

    void onReceive(const JsonDocument doc, const char *const source) override;
};

extern SignalExtension *Signal;

#endif // EXTENSION_SIGNAL
