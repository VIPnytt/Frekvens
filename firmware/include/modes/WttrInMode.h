#pragma once

#include "config/constants.h"

#if MODE_WTTRIN

#include <vector>

#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

class WttrInMode : public ModeModule
{
private:
    static constexpr uint32_t interval = 3'600'000; // Recommended update interval: 1 hour (due to heavy caching)

    unsigned long lastMillis = 0;

    // https://wttr.in/:help
    // https://github.com/chubin/wttr.in#readme
    std::vector<const char *> urls = {
        "https://wttr.in/?format=j2",
#ifdef LOCATION
        "https://wttr.in/" LOCATION "?format=j2",
#endif
#if defined(LATITUDE) && defined(LONGITUDE)
        "https://wttr.in/" LATITUDE "," LONGITUDE "?format=j2",
#endif
    };

    // https://github.com/chubin/wttr.in/blob/master/lib/constants.py
    const std::vector<WeatherHandler::Codeset16> codesets = {
        {
            WeatherHandler::Conditions::CLEAR,
            {113},
        },
        {
            WeatherHandler::Conditions::CLOUDY,
            {119, 122},
        },
        {
            WeatherHandler::Conditions::CLOUDY_PARTLY,
            {116},
        },
        {
            WeatherHandler::Conditions::FOG,
            {143, 248, 260},
        },
        {
            WeatherHandler::Conditions::RAIN,
            {176, 293, 296, 299, 302, 305, 308, 311, 314, 353, 356, 359, 386, 389, 263, 266, 281, 284, 185},
        },
        {
            WeatherHandler::Conditions::SNOW,
            {179, 227, 323, 326, 329, 332, 335, 338, 368, 371, 392, 395, 230, 350},
        },
        {
            WeatherHandler::Conditions::THUNDER,
            {200, 386, 389, 392, 395},
        },
    };

    void update();

public:
    WttrInMode() : ModeModule("Wttr.in") {};

    void wake() override;
    void handle() override;
};

#endif // MODE_WTTRIN
