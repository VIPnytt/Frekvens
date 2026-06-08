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
    static constexpr std::array<uint16_t, 1U> codesClear{113U};
    static constexpr std::array<uint16_t, 2U> codesCloudy{119U, 122U};
    static constexpr std::array<uint16_t, 1U> codesCloudyPartly{116U};
    static constexpr std::array<uint16_t, 3U> codesFog{143U, 248U, 260U};
    static constexpr std::array<uint16_t, 9U> codesRain{176U, 263U, 266U, 293U, 296U, 299U, 302U, 305U, 308U};
    static constexpr std::array<uint16_t, 11U> codesSnow{
        179U, 182U, 185U, 227U, 230U, 281U, 284U, 311U, 314U, 317U, 320U};
    static constexpr std::array<uint16_t, 1U> codesThunder{200U};

    static constexpr std::array<Codeset16, 7U> codesets{{
        {Conditions::CLEAR, codesClear},
        {Conditions::CLOUDY, codesCloudy},
        {Conditions::CLOUDY_PARTLY, codesCloudyPartly},
        {Conditions::FOG, codesFog},
        {Conditions::RAIN, codesRain},
        {Conditions::SNOW, codesSnow},
        {Conditions::THUNDER, codesThunder},
    }};

    // https://www.worldweatheronline.com/weather-api/api/docs/local-city-town-weather-api.aspx
    static inline std::vector<const char *> queries{
#ifdef LOCATION
        "q=" LOCATION "&cc=yes&fx=no&mca=no&format=json&key=" WORLDWEATHERONLINE_KEY,
#endif
#if defined(LATITUDE) && defined(LONGITUDE)
        "q=" LATITUDE "," LONGITUDE "&cc=yes&fx=no&mca=no&format=json&key=" WORLDWEATHERONLINE_KEY,
#endif
    };

public:
    static constexpr std::string_view name{"World Weather Online"};

    explicit WorldWeatherOnlineMiddleware() : WeatherHandler(name)
    {
        host = "api.worldweatheronline.com";
        path = "/premium/v1/weather.ashx";
    };

    void update(std::optional<WeatherHandler::Conditions> &condition, std::optional<int16_t> &temperature,
                unsigned long &lastMillis) override;
};

#endif // WEATHER_WORLDWEATHERONLINE
