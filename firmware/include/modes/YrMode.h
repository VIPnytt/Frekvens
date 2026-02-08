#pragma once

#if MODE_YR

#include "config/constants.h"
#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

#include <vector>

class YrMode final : public ModeModule
{
private:
    static constexpr uint32_t interval = 300'000; // Data resolution: down to 5 minutes (depending on location)

    unsigned long lastMillis = 0;

    // https://api.met.no/weatherapi/locationforecast/2.0/documentation
    // https://api.met.no/weatherapi/nowcast/2.0/documentation
    std::vector<const char *> urls = {
        "https://api.met.no/weatherapi/locationforecast/2.0/complete?lat=" LATITUDE "&lon=" LONGITUDE,
        "https://api.met.no/weatherapi/nowcast/2.0/complete?lat=" LATITUDE "&lon=" LONGITUDE,
    };

    // https://github.com/metno/weathericons/tree/main/weather
    const std::vector<WeatherHandler::Codeset> codesets = {
        {
            WeatherHandler::Conditions::CLEAR,
            {
                "clearsky_day",
                "clearsky_night",
                "clearsky_polartwilight",
                "fair_day",
                "fair_night",
                "fair_polartwilight",
            },
        },
        {
            WeatherHandler::Conditions::CLOUDY,
            {
                "cloudy",
            },
        },
        {
            WeatherHandler::Conditions::CLOUDY_PARTLY,
            {
                "partlycloudy_day",
                "partlycloudy_night",
                "partlycloudy_polartwilight",
            },
        },
        {
            WeatherHandler::Conditions::FOG,
            {
                "fog",
            },
        },
        {
            WeatherHandler::Conditions::RAIN,
            {
                "heavyrain",
                "heavyrainshowers_day",
                "heavyrainshowers_night",
                "heavyrainshowers_polartwilight",
                "lightrain",
                "lightrainshowers_day",
                "lightrainshowers_night",
                "lightrainshowers_polartwilight",
                "rain",
                "rainshowers_day",
                "rainshowers_night",
                "rainshowers_polartwilight",
            },
        },
        {
            WeatherHandler::Conditions::SNOW,
            {
                "heavysleet",
                "heavysleetshowers_day",
                "heavysleetshowers_night",
                "heavysleetshowers_polartwilight",
                "heavysnow",
                "heavysnowshowers_day",
                "heavysnowshowers_night",
                "heavysnowshowers_polartwilight"
                "lightsleet",
                "lightsleetshowers_day",
                "lightsleetshowers_night",
                "lightsleetshowers_polartwilight",
                "lightsnow",
                "lightsnowshowers_day",
                "lightsnowshowers_night",
                "lightsnowshowers_polartwilight",
                "sleet",
                "sleetshowers_day",
                "sleetshowers_night",
                "sleetshowers_polartwilight",
                "snow",
                "snowshowers_day",
                "snowshowers_night",
                "snowshowers_polartwilight",
            },
        },
        {
            WeatherHandler::Conditions::THUNDER,
            {
                "heavyrainandthunder",
                "heavyrainshowersandthunder_day",
                "heavyrainshowersandthunder_night",
                "heavyrainshowersandthunder_polartwilight",
                "heavysleetandthunder",
                "heavysleetshowersandthunder_day",
                "heavysleetshowersandthunder_night",
                "heavysleetshowersandthunder_polartwilight",
                "heavysnowandthunder",
                "heavysnowshowersandthunder_day",
                "heavysnowshowersandthunder_night",
                "heavysnowshowersandthunder_polartwilight",
                "lightrainandthunder",
                "lightrainshowersandthunder_day",
                "lightrainshowersandthunder_night",
                "lightrainshowersandthunder_polartwilight",
                "lightsleetandthunder",
                "lightsnowandthunder",
                "lightsleetshowersandthunder_day",
                "lightsleetshowersandthunder_night",
                "lightsleetshowersandthunder_polartwilight",
                "lightssleetshowersandthunder_day",
                "lightssleetshowersandthunder_night",
                "lightssleetshowersandthunder_polartwilight",
                "lightsnowshowersandthunder_day",
                "lightsnowshowersandthunder_night",
                "lightsnowshowersandthunder_polartwilight",
                "lightssnowshowersandthunder_day",
                "lightssnowshowersandthunder_night",
                "lightssnowshowersandthunder_polartwilight",
                "rainandthunder",
                "rainshowersandthunder_day",
                "rainshowersandthunder_night",
                "rainshowersandthunder_polartwilight",
                "sleetandthunder",
                "sleetshowersandthunder_day",
                "sleetshowersandthunder_night",
                "sleetshowersandthunder_polartwilight",
                "snowandthunder",
                "snowshowersandthunder_day",
                "snowshowersandthunder_night",
                "snowshowersandthunder_polartwilight",
            },
        },
    };

    void update();

public:
    explicit YrMode() : ModeModule("Yr") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_YR
