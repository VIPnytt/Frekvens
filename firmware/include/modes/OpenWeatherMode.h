#pragma once

#include "config/constants.h"

#if defined(OPENWEATHER_KEY) && defined(LATITUDE) && defined(LONGITUDE)

#include <vector>

#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

class OpenWeatherMode : public ModeModule
{
private:
    unsigned long lastMillis = 0;

    // https://openweathermap.org/api/one-call-3#current
    // https://openweathermap.org/current#one
    std::vector<const char *> urls = {
        "https://api.openweathermap.org/data/3.0/onecall?lat=" LATITUDE "&lon=" LONGITUDE "&exclude=alerts,daily,hourly,minutely&appid=" OPENWEATHER_KEY,
        "https://api.openweathermap.org/data/2.5/weather?lat=" LATITUDE "&lon=" LONGITUDE "&appid=" OPENWEATHER_KEY,
#ifdef OPENWEATHER_PARAMETERS
        "https://api.openweathermap.org/data/3.0/onecall?lat=" LATITUDE "&lon=" LONGITUDE "&exclude=alerts,daily,hourly,minutely&appid=" OPENWEATHER_KEY "&" OPENWEATHER_PARAMETERS,
        "https://api.openweathermap.org/data/2.5/weather?lat=" LATITUDE "&lon=" LONGITUDE "&appid=" OPENWEATHER_KEY "&" OPENWEATHER_PARAMETERS,
#endif
    };

    // https://openweathermap.org/weather-conditions#Weather-Condition-Codes-2
    const std::vector<WeatherHandler::Codeset16> codesets = {
        {
            WeatherHandler::Conditions::CLEAR,
            {800},
        },
        {
            WeatherHandler::Conditions::CLOUDY,
            {804},
        },
        {
            WeatherHandler::Conditions::CLOUDY_PARTLY,
            {801, 802, 803},
        },
        {
            WeatherHandler::Conditions::FOG,
            {701, 711, 721, 731, 741, 751, 761, 762, 771, 781},
        },
        {
            WeatherHandler::Conditions::RAIN,
            {300, 301, 302, 310, 311, 312, 313, 314, 321, 500, 501, 502, 503, 504, 511, 520, 521, 522, 531},
        },
        {
            WeatherHandler::Conditions::SNOW,
            {600, 601, 602, 611, 612, 613, 615, 616, 620, 621, 622},
        },
        {
            WeatherHandler::Conditions::THUNDER,
            {200, 201, 202, 210, 211, 212, 221, 230, 231, 232},
        },
    };

    void update();

public:
    OpenWeatherMode() : ModeModule("OpenWeather") {};

#if EXTENSION_BUILD
    void setup() override;
#endif
    void wake() override;
    void handle() override;
};

#endif // defined(LATITUDE) && defined(LONGITUDE) && defined(OPENWEATHER_KEY)
