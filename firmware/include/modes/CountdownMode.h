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
    static constexpr std::array<std::string_view, 4U> fontNames{
        MiniFont::name,
        MediumFont::name,
        MediumBoldFont::name,
        MediumWideFont::name,
    };

    static inline std::chrono::time_point<std::chrono::system_clock> epoch{};

    static inline std::string fontName{fontNames[0U]};

    bool odd{false};

    uint8_t blink{0U};
    uint8_t lower{0U};
    uint8_t upper{0U};

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

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif
};

#endif // MODE_COUNTDOWN
