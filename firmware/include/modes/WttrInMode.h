#pragma once

#if MODE_WTTRIN

#include "config/constants.h"
#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

#include <vector>

class WttrInMode final : public ModeModule
{
private:
    static constexpr uint32_t interval = 3'600'000; // Recommended update interval: 1 hour (due to heavy caching)

    unsigned long lastMillis = 0;

    // https://wttr.in/:help
    // https://github.com/chubin/wttr.in#readme
    std::vector<const char *> urls{
        "https://wttr.in/?format=j2",
#ifdef LOCATION
        "https://wttr.in/" LOCATION "?format=j2",
#endif
#if defined(LATITUDE) && defined(LONGITUDE)
        "https://wttr.in/" LATITUDE "," LONGITUDE "?format=j2",
#endif
    };

    // https://github.com/chubin/wttr.in/blob/master/lib/constants.py
    static inline constexpr std::array<uint16_t, 1> codesClear{113};
    static inline constexpr std::array<uint16_t, 2> codesCloudy{119, 122};
    static inline constexpr std::array<uint16_t, 1> codesCloudyPartly{116};
    static inline constexpr std::array<uint16_t, 3> codesFog{143, 248, 260};
    static inline constexpr std::array<uint16_t, 19> codesRain{
        176, 293, 296, 299, 302, 305, 308, 311, 314, 353, 356, 359, 386, 389, 263, 266, 281, 284, 185};
    static inline constexpr std::array<uint16_t, 14> codesSnow{
        179, 227, 323, 326, 329, 332, 335, 338, 368, 371, 392, 395, 230, 350};
    static inline constexpr std::array<uint16_t, 5> codesThunder{200, 386, 389, 392, 395};

    static constexpr std::array<WeatherHandler::Codeset16, 7> codesets{{
        {WeatherHandler::Conditions::CLEAR, codesClear},
        {WeatherHandler::Conditions::CLOUDY, codesCloudy},
        {WeatherHandler::Conditions::CLOUDY_PARTLY, codesCloudyPartly},
        {WeatherHandler::Conditions::FOG, codesFog},
        {WeatherHandler::Conditions::RAIN, codesRain},
        {WeatherHandler::Conditions::SNOW, codesSnow},
        {WeatherHandler::Conditions::THUNDER, codesThunder},
    }};

    void update();

public:
    explicit WttrInMode() : ModeModule("Wttr.in") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_WTTRIN
