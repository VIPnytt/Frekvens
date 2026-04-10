#pragma once

#if MODE_CLOCK

#include "config/constants.h"     // NOLINT(misc-include-cleaner)
#include "fonts/MediumBoldFont.h" // NOLINT(misc-include-cleaner)
#include "fonts/MediumFont.h"     // NOLINT(misc-include-cleaner)
#include "fonts/MediumWideFont.h" // NOLINT(misc-include-cleaner)
#include "fonts/MiniFont.h"       // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <bits/unique_ptr.h>
#include <vector>

class ClockMode final : public ModeModule
{
private:
    static constexpr std::array<std::string_view, 4> fontNames{
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

    uint8_t fontHeigt = 0;

    std::string fontName = MediumBoldFont::name.data();

    void drawDigits();
    void drawTicker();
    void setFont(std::string_view _fontName);
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
