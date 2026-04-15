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
    static constexpr auto fontNames = std::to_array<std::string_view>({
#if FONT_MINI
        MiniFont::name,
#endif
#if FONT_MEDIUM
        MediumFont::name,
#endif
#if FONT_MEDIUMBOLD
        MediumBoldFont::name,
#endif
#if FONT_MEDIUMWIDE
        MediumWideFont::name,
#endif
    });

    tm local{};

    bool pending = false;
    bool strikethrough = false;
    bool ticking = true;

    int hour = 24;
    int minute = 60;
    int second = 60;

    std::string fontName = fontNames[0].data();

    void drawDigits();
    void drawTicker();
    void setFont(std::string_view _fontName);
    void setTicking(bool _ticking);
    void transmit();

public:
    static constexpr std::string_view name{"Clock"};

    explicit ClockMode() : ModeModule(name) {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(JsonObjectConst payload, std::string_view source) override;
};

#endif // MODE_CLOCK
