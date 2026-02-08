#pragma once

#if MODE_GOOGLEWEATHER

#include "config/constants.h"
#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

#include <vector>

class GoogleWeatherMode : public ModeModule
{
private:
    static constexpr uint32_t interval = 900'000; // Update interval: 15 minutes

    unsigned long lastMillis = 0;

    // https://developers.google.com/maps/documentation/weather
    std::vector<const char *> urls = {
        "https://weather.googleapis.com/v1/currentConditions:lookup?location.latitude=" LATITUDE
        "&location.longitude=" LONGITUDE "&key=" GOOGLEWEATHER_KEY,
#if TEMPERATURE_CELSIUS
        "https://weather.googleapis.com/v1/currentConditions:lookup?location.latitude=" LATITUDE
        "&location.longitude=" LONGITUDE "&unitsSystem=METRIC&key=" GOOGLEWEATHER_KEY,
#elif TEMPERATURE_FAHRENHEIT
        "https://weather.googleapis.com/v1/currentConditions:lookup?location.latitude=" LATITUDE
        "&location.longitude=" LONGITUDE "&unitsSystem=IMPERIAL&key=" GOOGLEWEATHER_KEY,
#endif // TEMPERATURE_CELSIUS
    };

    // https://developers.google.com/maps/documentation/weather/weather-condition-icons
    const std::vector<WeatherHandler::Codeset> codesets = {
        {
            WeatherHandler::Conditions::CLEAR,
            {
                "CLEAR",
                "MOSTLY_CLEAR",
            },
        },
        {
            WeatherHandler::Conditions::CLOUDY,
            {
                "CLOUDY",
            },
        },
        {
            WeatherHandler::Conditions::CLOUDY_PARTLY,
            {
                "MOSTLY_CLOUDY",
                "PARTLY_CLOUDY",
            },
        },
        {
            WeatherHandler::Conditions::RAIN,
            {
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
            },
        },
        {
            WeatherHandler::Conditions::SNOW,
            {
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
            },
        },
        {
            WeatherHandler::Conditions::THUNDER,
            {
                "HEAVY_SNOW_STORM",
                "HEAVY_THUNDERSTORM",
                "LIGHT_THUNDERSTORM_RAIN",
                "SCATTERED_THUNDERSTORMS",
                "SNOWSTORM",
                "THUNDERSHOWER",
                "THUNDERSTORM",
            },
        },
        {
            WeatherHandler::Conditions::WIND,
            {
                "BLOWING_SNOW",
                "WIND_AND_RAIN",
                "WINDY",
            },
        },
    };

    void update();

public:
    GoogleWeatherMode() : ModeModule("Google Weather") {};

    void begin();
    void handle();
};

#endif // MODE_GOOGLEWEATHER
