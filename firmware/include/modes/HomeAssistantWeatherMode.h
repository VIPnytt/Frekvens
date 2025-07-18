#pragma once

#include "config/constants.h"

#if MODE_HOMEASSISTANTWEATHER

#include <vector>

#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

class HomeAssistantWeatherMode : public ModeModule
{
private:
    unsigned long lastMillis = 0;

    // https://developers.home-assistant.io/docs/api/rest
    std::vector<std::string> urls = {
#ifdef HOMEASSISTANT_PORT
        std::string("http://").append(HOMEASSISTANT_HOST ":").append(std::to_string(HOMEASSISTANT_PORT)).append("/api/states/weather.forecast_home"),
#ifdef HOMEASSISTANT_ENTITY
        std::string("http://").append(HOMEASSISTANT_HOST ":").append(std::to_string(HOMEASSISTANT_PORT)).append("/api/states/" HOMEASSISTANT_ENTITY),
#endif // HOMEASSISTANT_ENTITY
#else
        std::string("http://").append(HOMEASSISTANT_HOST).append("/api/states/weather.forecast_home"),
#ifdef HOMEASSISTANT_ENTITY
        std::string("http://").append(HOMEASSISTANT_HOST).append("/api/states/" HOMEASSISTANT_ENTITY),
#endif // HOMEASSISTANT_ENTITY
#endif // HOMEASSISTANT_PORT

    };

    // https://www.home-assistant.io/integrations/weather/#condition-mapping
    const std::vector<WeatherHandler::Codeset> codesets = {
        {
            WeatherHandler::Conditions::CLEAR,
            {
                "clear-night",
                "sunny",
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
                "partlycloudy",
            },
        },
        {
            WeatherHandler::Conditions::EXCEPTION,
            {
                "exceptional",
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
                "pouring",
                "rainy",
            },
        },
        {
            WeatherHandler::Conditions::SNOW,
            {
                "hail",
                "snowy",
                "snowy-rainy",
            },
        },
        {
            WeatherHandler::Conditions::THUNDER,
            {
                "lighting",
                "lightning-rainy",
            },
        },
        {
            WeatherHandler::Conditions::WIND,
            {
                "windy",
                "windy-variant",
            },
        },
    };

    void update();

public:
    HomeAssistantWeatherMode() : ModeModule("Home Assistant weather") {};

#if EXTENSION_BUILD
    void setup() override;
#endif
    void wake() override;
    void handle() override;
};

#endif // MODE_HOMEASSISTANTWEATHER
