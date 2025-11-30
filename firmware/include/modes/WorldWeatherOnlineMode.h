#pragma once

#include "config/constants.h"

#if MODE_WORLDWEATHERONLINE

#include <vector>

#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

class WorldWeatherOnlineMode : public ModeModule
{
private:
    static constexpr uint32_t interval = 600'000; // Recommended update interval: 10-15 minutes

    unsigned long lastMillis = 0;

    // https://www.worldweatheronline.com/weather-api/api/docs/local-city-town-weather-api.aspx
    std::vector<const char *> urls = {
#ifdef LOCATION
        "https://api.worldweatheronline.com/premium/v1/weather.ashx?q=" LOCATION "&cc=yes&fx=no&mca=no&format=json&key=" WORLDWEATHERONLINE_KEY,
#endif
#if defined(LATITUDE) && defined(LONGITUDE)
        "https://api.worldweatheronline.com/premium/v1/weather.ashx?q=" LATITUDE "," LONGITUDE "&cc=yes&fx=no&mca=no&format=json&key=" WORLDWEATHERONLINE_KEY,
#endif
    };

    // https://www.worldweatheronline.com/weather-api/api/docs/weather-icons.aspx
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
            {176, 263, 266, 293, 296, 299, 302, 305, 308},
        },
        {
            WeatherHandler::Conditions::SNOW,
            {179, 182, 185, 227, 230, 281, 284, 311, 314, 317, 320},
        },
        {
            WeatherHandler::Conditions::THUNDER,
            {200},
        },
    };

    void update();

public:
    WorldWeatherOnlineMode() : ModeModule("World Weather Online") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_WORLDWEATHERONLINE
