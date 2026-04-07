#pragma once

#if WEATHER_WORLDWEATHERONLINE

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "handlers/WeatherHandler.h"

#include <array>
#include <string_view>
#include <vector>

class WorldWeatherOnlineMiddleware final : public WeatherHandler
{
private:
    // https://www.worldweatheronline.com/weather-api/api/docs/weather-icons.aspx
    static constexpr std::array<uint16_t, 1> codesClear{113};
    static constexpr std::array<uint16_t, 2> codesCloudy{119, 122};
    static constexpr std::array<uint16_t, 1> codesCloudyPartly{116};
    static constexpr std::array<uint16_t, 3> codesFog{143, 248, 260};
    static constexpr std::array<uint16_t, 9> codesRain{176, 263, 266, 293, 296, 299, 302, 305, 308};
    static constexpr std::array<uint16_t, 11> codesSnow{179, 182, 185, 227, 230, 281, 284, 311, 314, 317, 320};
    static constexpr std::array<uint16_t, 1> codesThunder{200};

    static constexpr std::array<Codeset16, 7> codesets{{
        {Conditions::CLEAR, codesClear},
        {Conditions::CLOUDY, codesCloudy},
        {Conditions::CLOUDY_PARTLY, codesCloudyPartly},
        {Conditions::FOG, codesFog},
        {Conditions::RAIN, codesRain},
        {Conditions::SNOW, codesSnow},
        {Conditions::THUNDER, codesThunder},
    }};

    // https://www.worldweatheronline.com/weather-api/api/docs/local-city-town-weather-api.aspx
    inline static std::vector<const char *> queries{
#ifdef LOCATION
        "q=" LOCATION "&cc=yes&fx=no&mca=no&format=json&key=" WORLDWEATHERONLINE_KEY,
#endif
#if defined(LATITUDE) && defined(LONGITUDE)
        "q=" LATITUDE "," LONGITUDE "&cc=yes&fx=no&mca=no&format=json&key=" WORLDWEATHERONLINE_KEY,
#endif
    };

public:
    static constexpr std::string_view name{"World Weather Online"};

    explicit WorldWeatherOnlineMiddleware() : WeatherHandler(name.data())
    {
        host = "api.worldweatheronline.com";
        path = "/premium/v1/weather.ashx";
    };

    void update(std::optional<WeatherHandler::Conditions> &condition, std::optional<int16_t> &temperature,
                unsigned long &lastMillis) override;
};

#endif // WEATHER_WORLDWEATHERONLINE
