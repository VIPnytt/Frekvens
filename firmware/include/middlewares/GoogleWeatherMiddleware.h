#pragma once

#if WEATHER_GOOGLE

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "handlers/WeatherHandler.h"

#include <array>
#include <string_view>
#include <vector>

class GoogleWeatherMiddleware final : public WeatherHandler
{
private:
    // https://developers.google.com/maps/documentation/weather/weather-condition-icons
    static constexpr std::array<std::string_view, 2U> codesClear{
        "CLEAR",
        "MOSTLY_CLEAR",
    };
    static constexpr std::array<std::string_view, 1U> codesCloudy{
        "CLOUDY",
    };
    static constexpr std::array<std::string_view, 2U> codesCloudyPartly{
        "MOSTLY_CLOUDY",
        "PARTLY_CLOUDY",
    };
    static constexpr std::array<std::string_view, 11U> codesRain{
        "CHANCE_OF_SHOWERS",
        "HEAVY_RAIN",
        "HEAVY_RAIN_SHOWERS",
        "LIGHT_RAIN",
        "LIGHT_RAIN_SHOWERS",
        "LIGHT_TO_MODERATE_RAIN",
        "MODERATE_TO_HEAVY_RAIN",
        "RAIN",
        "RAIN_PERIODICALLY_HEAVY",
        "RAIN_SHOWERS",
        "SCATTERED_SHOWERS",
    };
    static constexpr std::array<std::string_view, 14U> codesSnow{
        "CHANCE_OF_SNOW_SHOWERS",
        "HAIL",
        "HAIL_SHOWERS",
        "HEAVY_SNOW",
        "HEAVY_SNOW_SHOWERS",
        "LIGHT_SNOW",
        "LIGHT_SNOW_SHOWERS",
        "LIGHT_TO_MODERATE_SNOW",
        "MODERATE_TO_HEAVY_SNOW",
        "RAIN_AND_SNOW",
        "SCATTERED_SNOW_SHOWERS",
        "SNOW",
        "SNOW_PERIODICALLY_HEAVY",
        "SNOW_SHOWERS",
    };
    static constexpr std::array<std::string_view, 7U> codesThunder{
        "HEAVY_SNOW_STORM",
        "HEAVY_THUNDERSTORM",
        "LIGHT_THUNDERSTORM_RAIN",
        "SCATTERED_THUNDERSTORMS",
        "SNOWSTORM",
        "THUNDERSHOWER",
        "THUNDERSTORM",
    };
    static constexpr std::array<std::string_view, 3U> codesWind{
        "BLOWING_SNOW",
        "WIND_AND_RAIN",
        "WINDY",
    };

    static constexpr std::array<std::pair<WeatherHandler::Condition, std::span<const std::string_view>>, 7U> codesets{{
        {WeatherHandler::Condition::CLEAR, codesClear},
        {WeatherHandler::Condition::CLOUDY, codesCloudy},
        {WeatherHandler::Condition::CLOUDY_PARTLY, codesCloudyPartly},
        {WeatherHandler::Condition::RAIN, codesRain},
        {WeatherHandler::Condition::SNOW, codesSnow},
        {WeatherHandler::Condition::THUNDER, codesThunder},
        {WeatherHandler::Condition::WIND, codesWind},
    }};

    // https://developers.google.com/maps/documentation/weather
    static inline std::vector<const char *> queries{
#if TEMPERATURE_CELSIUS || TEMPERATURE_KELVIN
        "location.latitude=" LATITUDE "&location.longitude=" LONGITUDE "&unitsSystem=METRIC&key=" GOOGLEWEATHER_KEY,
#elif TEMPERATURE_FAHRENHEIT
        "location.latitude=" LATITUDE "&location.longitude=" LONGITUDE "&unitsSystem=IMPERIAL&key=" GOOGLEWEATHER_KEY,
#else
        "location.latitude=" LATITUDE "&location.longitude=" LONGITUDE "&key=" GOOGLEWEATHER_KEY,
#endif // TEMPERATURE_CELSIUS || TEMPERATURE_KELVIN
    };

public:
    static constexpr std::string_view name{"Google"};

    explicit GoogleWeatherMiddleware() : WeatherHandler(name)
    {
        host = "weather.googleapis.com";
        path = "/v1/currentConditions:lookup";
    };

    void update(std::optional<WeatherHandler::Condition> &condition, std::optional<int16_t> &temperature,
                unsigned long &lastMillis) override;
};

#endif // WEATHER_GOOGLE
