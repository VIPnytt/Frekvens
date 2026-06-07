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
    static constexpr std::array<uint16_t, 1U> codesClear{800U};
    static constexpr std::array<uint16_t, 1U> codesCloudy{804U};
    static constexpr std::array<uint16_t, 3U> codesCloudyPartly{801U, 802U, 803U};
    static constexpr std::array<uint16_t, 10U> codesFog{701U, 711U, 721U, 731U, 741U, 751U, 761U, 762U, 771U, 781U};
    static constexpr std::array<uint16_t, 19U> codesRain{
        300U,
        301U,
        302U,
        310U,
        311U,
        312U,
        313U,
        314U,
        321U,
        500U,
        501U,
        502U,
        503U,
        504U,
        511U,
        520U,
        521U,
        522U,
        531U,
    };
    static constexpr std::array<uint16_t, 11U> codesSnow{
        600U, 601U, 602U, 611U, 612U, 613U, 615U, 616U, 620U, 621U, 622U};
    static constexpr std::array<uint16_t, 10U> codesThunder{200U, 201U, 202U, 210U, 211U, 212U, 221U, 230U, 231U, 232U};

    static constexpr std::array<WeatherHandler::Codeset16, 7U> codesets{{
        {WeatherHandler::Conditions::CLEAR, codesClear},
        {WeatherHandler::Conditions::CLOUDY, codesCloudy},
        {WeatherHandler::Conditions::CLOUDY_PARTLY, codesCloudyPartly},
        {WeatherHandler::Conditions::FOG, codesFog},
        {WeatherHandler::Conditions::RAIN, codesRain},
        {WeatherHandler::Conditions::SNOW, codesSnow},
        {WeatherHandler::Conditions::THUNDER, codesThunder},
    }};

    // https://openweathermap.org/api/one-call-4?collection=one_call_api#current
    // https://openweathermap.org/api/one-call-3?collection=one_call_api#current
    // https://openweathermap.org/api/current?collection=current_forecast#one
    static inline std::vector<std::pair<const char *, const char *>> parts{
#if defined(LOCATION) && TEMPERATURE_CELSIUS
        {
            "/data/2.5/weather",
            "q=" LOCATION "&units=metric&appid=" OPENWEATHER_KEY,
        },
#elif defined(LOCATION) && TEMPERATURE_FAHRENHEIT
        {
            "/data/2.5/weather",
            "q=" LOCATION "&units=imperial&appid=" OPENWEATHER_KEY,
        },
#elif defined(LOCATION) && TEMPERATURE_KELVIN
        {
            "/data/2.5/weather",
            "q=" LOCATION "&units=standard&appid=" OPENWEATHER_KEY,
        },
#elif defined(LOCATION)
        {
            "/data/2.5/weather",
            "q=" LOCATION "&appid=" OPENWEATHER_KEY,
        },
#endif // defined(LOCATION) && TEMPERATURE_CELSIUS
#if defined(LATITUDE) && defined(LONGITUDE) && TEMPERATURE_CELSIUS
        {
            "/data/3.0/onecall",
            "lat=" LATITUDE "&lon=" LONGITUDE
            "&exclude=alerts,daily,hourly,minutely&units=metric&appid=" OPENWEATHER_KEY,
        },
        {
            "/data/4.0/onecall/current",
            "lat=" LATITUDE "&lon=" LONGITUDE "&units=metric&appid=" OPENWEATHER_KEY,
        },
        {
            "/data/2.5/weather",
            "lat=" LATITUDE "&lon=" LONGITUDE "&units=metric&appid=" OPENWEATHER_KEY,
        },
#elif defined(LATITUDE) && defined(LONGITUDE) && TEMPERATURE_FAHRENHEIT
        {
            "/data/3.0/onecall",
            "lat=" LATITUDE "&lon=" LONGITUDE
            "&exclude=alerts,daily,hourly,minutely&units=imperial&appid=" OPENWEATHER_KEY,
        },
        {
            "/data/4.0/onecall/current",
            "lat=" LATITUDE "&lon=" LONGITUDE "&units=imperial&appid=" OPENWEATHER_KEY,
        },
        {
            "/data/2.5/weather",
            "lat=" LATITUDE "&lon=" LONGITUDE "&units=imperial&appid=" OPENWEATHER_KEY,
        },
#elif defined(LATITUDE) && defined(LONGITUDE) && TEMPERATURE_KELVIN
        {
            "/data/3.0/onecall",
            "lat=" LATITUDE "&lon=" LONGITUDE
            "&exclude=alerts,daily,hourly,minutely&units=standard&appid=" OPENWEATHER_KEY,
        },
        {
            "/data/4.0/onecall/current",
            "lat=" LATITUDE "&lon=" LONGITUDE "&units=standard&appid=" OPENWEATHER_KEY,
        },
        {
            "/data/2.5/weather",
            "lat=" LATITUDE "&lon=" LONGITUDE "&units=standard&appid=" OPENWEATHER_KEY,
        },
#elif defined(LATITUDE) && defined(LONGITUDE)
        {
            "/data/3.0/onecall",
            "lat=" LATITUDE "&lon=" LONGITUDE "&exclude=alerts,daily,hourly,minutely&appid=" OPENWEATHER_KEY,
        },
        {
            "/data/4.0/onecall/current",
            "lat=" LATITUDE "&lon=" LONGITUDE "&appid=" OPENWEATHER_KEY,
        },
        {
            "/data/2.5/weather",
            "lat=" LATITUDE "&lon=" LONGITUDE "&appid=" OPENWEATHER_KEY,
        },
#endif // defined(LATITUDE) && defined(LONGITUDE) && TEMPERATURE_CELSIUS
    };

public:
    static constexpr std::string_view name{"OpenWeather"};

    explicit OpenWeatherMiddleware() : WeatherHandler(name) { host = "api.openweathermap.org"; };

    void update(std::optional<WeatherHandler::Conditions> &condition, std::optional<int16_t> &temperature,
                unsigned long &lastMillis) override;
};

#endif // WEATHER_OPENWEATHER
