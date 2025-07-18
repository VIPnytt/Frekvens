#pragma once

#include <vector>

#include "config/constants.h"
#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

class WttrInMode : public ModeModule
{
private:
    unsigned long lastMillis = 0;

    // https://wttr.in/:help
    // https://github.com/chubin/wttr.in#readme
    std::vector<const char *> urls = {
        "https://wttr.in/?format=j2",
#ifdef WTTRIN_PARAMETERS
        "https://wttr.in/?format=j2&" WTTRIN_PARAMETERS,
#endif
#ifdef LOCATION
        "https://wttr.in/" LOCATION "?format=j2",
#endif
#if defined(LOCATION) && defined(WTTRIN_PARAMETERS)
        "https://wttr.in/" LOCATION "?format=j2&" WTTRIN_PARAMETERS,
#endif
#if defined(LATITUDE) && defined(LONGITUDE)
        "https://wttr.in/" LATITUDE "," LONGITUDE "?format=j2",
#endif
#if defined(LATITUDE) && defined(LONGITUDE) && defined(WTTRIN_PARAMETERS)
        "https://wttr.in/" LATITUDE "," LONGITUDE "?format=j2&" WTTRIN_PARAMETERS,
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

#if EXTENSION_BUILD && defined(WTTRIN_PARAMETERS)
    void setup();
#endif
    void wake() override;
    void handle() override;
};
