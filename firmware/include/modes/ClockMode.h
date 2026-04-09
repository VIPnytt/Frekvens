#pragma once

#if MODE_CLOCK

#include "fonts/MediumBoldFont.h"
#include "fonts/MediumFont.h"
#include "fonts/MediumWideFont.h"
#include "fonts/MiniFont.h"
#include "modules/ModeModule.h"

#include <bits/unique_ptr.h>
#include <vector>

class ClockMode final : public ModeModule
{
private:
    static constexpr std::array<std::string_view, 4> fonts{
        MiniFont::name,
        MediumFont::name,
        MediumBoldFont::name,
        MediumWideFont::name,
    };

    tm local{};

    bool pending = false;
    bool ticking = false;

    int hour = 24;
    int minute = 60;
    int second = 60;

    uint8_t cellSize = 7;

    std::unique_ptr<FontModule> font{};

    void borderPixel(uint8_t sec, uint8_t brightness);
    void drawDigits();
    void setFont(std::string_view fontName);
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
