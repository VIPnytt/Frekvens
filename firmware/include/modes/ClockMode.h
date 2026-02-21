#pragma once

#if MODE_CLOCK

#include "modules/FontModule.h"
#include "modules/ModeModule.h"

#include <vector>

class ClockMode final : public ModeModule
{
private:
    tm local = {};

    bool pending = false;
    bool ticking = true;

    int hour = 24;
    int minute = 60;
    int second = 60;

    uint8_t cellSize = 7;

    FontModule *font = nullptr;

    std::vector<FontModule *> fonts = {};

    void borderPixel(uint8_t sec, uint8_t brightness);
    void setFont(const char *fontName);
    void setTicking(bool _ticking);
    void transmit();

public:
    explicit ClockMode() : ModeModule("Clock") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(JsonObjectConst payload, const char *source) override;
};

#endif // MODE_CLOCK
