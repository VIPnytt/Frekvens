#pragma once

#if WEATHER_OPENWEATHER

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "handlers/WeatherHandler.h"

#include <array>
#include <string_view>
#include <vector>

class OpenWeatherMiddleware final : public WeatherHandler
{
private:
    // https://openweathermap.org/weather-conditions#Weather-Condition-Codes-2
    static constexpr std::array<uint16_t, 1> codesClear{800};
    static constexpr std::array<uint16_t, 1> codesCloudy{804};
    static constexpr std::array<uint16_t, 3> codesCloudyPartly{801, 802, 803};
    static constexpr std::array<uint16_t, 10> codesFog{701, 711, 721, 731, 741, 751, 761, 762, 771, 781};
    static constexpr std::array<uint16_t, 19> codesRain{
        300, 301, 302, 310, 311, 312, 313, 314, 321, 500, 501, 502, 503, 504, 511, 520, 521, 522, 531};
    static constexpr std::array<uint16_t, 11> codesSnow{600, 601, 602, 611, 612, 613, 615, 616, 620, 621, 622};
    static constexpr std::array<uint16_t, 10> codesThunder{200, 201, 202, 210, 211, 212, 221, 230, 231, 232};

    static constexpr std::array<WeatherHandler::Codeset16, 7> codesets{{
        {WeatherHandler::Conditions::CLEAR, codesClear},
        {WeatherHandler::Conditions::CLOUDY, codesCloudy},
        {WeatherHandler::Conditions::CLOUDY_PARTLY, codesCloudyPartly},
        {WeatherHandler::Conditions::FOG, codesFog},
        {WeatherHandler::Conditions::RAIN, codesRain},
        {WeatherHandler::Conditions::SNOW, codesSnow},
        {WeatherHandler::Conditions::THUNDER, codesThunder},
    }};

    // https://openweathermap.org/api/one-call-3#current
    // https://openweathermap.org/current#one
    inline static std::vector<std::pair<const char *, const char *>> parts{
        {
            "/data/3.0/onecall",
            "lat=" LATITUDE "&lon=" LONGITUDE "&exclude=alerts,daily,hourly,minutely&appid=" OPENWEATHER_KEY,
        },
        {
            "/data/2.5/weather",
            "lat=" LATITUDE "&lon=" LONGITUDE "&appid=" OPENWEATHER_KEY,
        },
#if TEMPERATURE_KELVIN
        {
            "/data/3.0/onecall",
            "lat=" LATITUDE "&lon=" LONGITUDE
            "&exclude=alerts,daily,hourly,minutely&units=standard&appid=" OPENWEATHER_KEY,
        },
        {
            "/data/2.5/weather",
            "lat=" LATITUDE "&lon=" LONGITUDE "&units=standard&appid=" OPENWEATHER_KEY,
        },
#elif TEMPERATURE_CELSIUS
        {
            "/data/3.0/onecall",
            "lat=" LATITUDE "&lon=" LONGITUDE
            "&exclude=alerts,daily,hourly,minutely&units=metric&appid=" OPENWEATHER_KEY,
        },
        {
            "/data/2.5/weather",
            "lat=" LATITUDE "&lon=" LONGITUDE "&units=metric&appid=" OPENWEATHER_KEY,
        },
#elif TEMPERATURE_FAHRENHEIT
        {
            "/data/3.0/onecall",
            "lat=" LATITUDE "&lon=" LONGITUDE
            "&exclude=alerts,daily,hourly,minutely&units=imperial&appid=" OPENWEATHER_KEY,
        },
        {
            "/data/2.5/weather",
            "lat=" LATITUDE "&lon=" LONGITUDE "&units=imperial&appid=" OPENWEATHER_KEY,
        },
#endif // TEMPERATURE_KELVIN
    };

public:
    static constexpr std::string_view name{"OpenWeather"};

    explicit OpenWeatherMiddleware() : WeatherHandler(name.data()) { host = "api.openweathermap.org"; };

    void update(std::optional<WeatherHandler::Conditions> &condition, std::optional<int16_t> &temperature,
                unsigned long &lastMillis) override;
};

#endif // WEATHER_OPENWEATHER
