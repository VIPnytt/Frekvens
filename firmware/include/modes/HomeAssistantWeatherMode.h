#pragma once

#include "config/constants.h"

#if MODE_HOMEASSISTANTWEATHER

#include <vector>

#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

class HomeAssistantWeatherMode : public ModeModule
{
private:
#if defined(HOMEASSISTANT_HOST) && defined(HOMEASSISTANT_PORT)
    static constexpr std::string_view authority = HOMEASSISTANT_HOST ":" HOMEASSISTANT_PORT;
#elif defined(HOMEASSISTANT_HOST)
    static constexpr std::string_view authority = HOMEASSISTANT_HOST ":8123";
#elif defined(HOMEASSISTANT_PORT)
    static constexpr std::string_view authority = "homeassistant.local:" HOMEASSISTANT_PORT;
#else
    static constexpr std::string_view authority = "homeassistant.local:8123";
#endif // defined(HOMEASSISTANT_HOST) && defined(HOMEASSISTANT_PORT)

    unsigned long lastMillis = 0;

    // https://developers.home-assistant.io/docs/api/rest
    std::vector<std::string> urls = {
        std::string("http://").append(authority).append("/api/states/weather.forecast_home"),
#ifdef HOMEASSISTANT_ENTITY
        std::string("http://").append(authority).append("/api/states/" HOMEASSISTANT_ENTITY),
#endif
        std::string("https://").append(authority).append("/api/states/weather.forecast_home"),
#ifdef HOMEASSISTANT_ENTITY
        std::string("https://").append(authority).append("/api/states/" HOMEASSISTANT_ENTITY),
#endif
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
