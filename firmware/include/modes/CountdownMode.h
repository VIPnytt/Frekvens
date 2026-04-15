#pragma once

#if MODE_COUNTDOWN

#include "config/constants.h"     // NOLINT(misc-include-cleaner)
#include "fonts/MediumBoldFont.h" // NOLINT(misc-include-cleaner)
#include "fonts/MediumFont.h"     // NOLINT(misc-include-cleaner)
#include "fonts/MediumWideFont.h" // NOLINT(misc-include-cleaner)
#include "fonts/MiniFont.h"       // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

#include <chrono>

class CountdownMode final : public ModeModule
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
    bool odd = false;

    uint8_t blink = 0;
    uint8_t lower = 0;
    uint8_t upper = 0;

    std::chrono::time_point<std::chrono::system_clock> epoch{};

    std::string fontName = fontNames[0].data();

    void save();
    void transmit();

public:
    static constexpr std::string_view name{"Countdown"};

    explicit CountdownMode() : ModeModule(name) {};

    void configure() override;
    void begin() override;
    void handle() override;
    void setFont(std::string_view _fontName);
    void onReceive(JsonObjectConst payload, std::string_view source) override;
};

#endif // MODE_COUNTDOWN
