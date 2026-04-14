#pragma once

#if WEATHER_YR

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "handlers/WeatherHandler.h"

#include <array>
#include <string_view>
#include <vector>

class YrMiddleware final : public WeatherHandler
{
private:
    // https://github.com/metno/weathericons/tree/main/weather
    static constexpr std::array<std::string_view, 6> codesClear{
        "clearsky_day",
        "clearsky_night",
        "clearsky_polartwilight",
        "fair_day",
        "fair_night",
        "fair_polartwilight",
    };
    static constexpr std::array<std::string_view, 1> codesCloudy{
        "cloudy",
    };
    static constexpr std::array<std::string_view, 3> codesCloudyPartly{
        "partlycloudy_day",
        "partlycloudy_night",
        "partlycloudy_polartwilight",
    };
    static constexpr std::array<std::string_view, 1> codesFog{
        "fog",
    };
    static constexpr std::array<std::string_view, 12> codesRain{
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
    };
    static constexpr std::array<std::string_view, 24> codesSnow{
        "heavysleet",
        "heavysleetshowers_day",
        "heavysleetshowers_night",
        "heavysleetshowers_polartwilight",
        "heavysnow",
        "heavysnowshowers_day",
        "heavysnowshowers_night",
        "heavysnowshowers_polartwilight",
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
    };
    static constexpr std::array<std::string_view, 42> codesThunder{
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
    };

    static constexpr std::array<Codeset, 7> codesets{{
        {Conditions::CLEAR, codesClear},
        {Conditions::CLOUDY, codesCloudy},
        {Conditions::CLOUDY_PARTLY, codesCloudyPartly},
        {Conditions::FOG, codesFog},
        {Conditions::RAIN, codesRain},
        {Conditions::SNOW, codesSnow},
        {Conditions::THUNDER, codesThunder},
    }};

    // https://api.met.no/weatherapi/locationforecast/2.0/documentation
    // https://api.met.no/weatherapi/nowcast/2.0/documentation
    inline static std::vector<const char *> paths{
        "/weatherapi/locationforecast/2.0/complete",
        "/weatherapi/nowcast/2.0/complete",
    };

public:
    static constexpr std::string_view name{"Yr"};

    explicit YrMiddleware() : WeatherHandler(name, 1U << 19U)
    {
        host = "api.met.no";
        query = "lat=" LATITUDE "&lon=" LONGITUDE;
    };

    void update(std::optional<WeatherHandler::Conditions> &condition, std::optional<int16_t> &temperature,
                unsigned long &lastMillis) override;
};

#endif // WEATHER_YR
