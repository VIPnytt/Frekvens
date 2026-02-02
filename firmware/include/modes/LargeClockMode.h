#pragma once

#if MODE_LARGECLOCK

#include "modules/FontModule.h"
#include "modules/ModeModule.h"

#include <vector>

class LargeClockMode final : public ModeModule
{
private:
    tm local = {};

    bool pending = false;
    bool ticking = false;

    int hour = 24;
    int minute = 60;
    int second = 0;

    FontModule *font = nullptr;

    std::vector<FontModule *> fonts = {};

    void setFont(const char *fontName);
    void setTicking(bool _ticking);
    void transmit();

protected:
    ~LargeClockMode() = default;

public:
    LargeClockMode() : ModeModule("Large clock") {};
    LargeClockMode(const LargeClockMode &) = delete;
    LargeClockMode &operator=(const LargeClockMode &) = delete;

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(const JsonDocument &doc, const char *source) override;
};

#endif // MODE_LARGECLOCK
