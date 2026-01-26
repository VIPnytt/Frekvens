#pragma once

#if MODE_LARGECLOCK

#include "modules/FontModule.h"
#include "modules/ModeModule.h"

#include <vector>

class LargeClockMode : public ModeModule
{
private:
    tm local;

    bool
        pending = false,
        ticking = false;

    uint8_t
        hour,
        minute,
        second = 0;

    FontModule *font = nullptr;

    std::vector<FontModule *> fonts = {};

    void setFont(const char *const fontName);
    void setTicking(const bool _ticking);
    void transmit();

public:
    LargeClockMode() : ModeModule("Large clock") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(const JsonDocument doc, const char *const source) override;
};

#endif // MODE_LARGECLOCK
