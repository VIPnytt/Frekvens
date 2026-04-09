#pragma once

#if WEATHER_HOMEASSISTANT

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "handlers/WeatherHandler.h"

#include <array>
#include <string_view>
#include <vector>

class HomeAssistantWeatherMiddleware final : public WeatherHandler
{
private:
    // https://www.home-assistant.io/integrations/weather/#condition-mapping
    static constexpr std::array<std::string_view, 2> codesClear{
        "clear-night",
        "sunny",
    };
    static constexpr std::array<std::string_view, 1> codesCloudy{
        "cloudy",
    };
    static constexpr std::array<std::string_view, 1> codesCloudyPartly{
        "partlycloudy",
    };
    static constexpr std::array<std::string_view, 1> codesException{
        "exceptional",
    };
    static constexpr std::array<std::string_view, 1> codesFog{
        "fog",
    };
    static constexpr std::array<std::string_view, 2> codesRain{
        "pouring",
        "rainy",
    };
    static constexpr std::array<std::string_view, 3> codesSnow{
        "hail",
        "snowy",
        "snowy-rainy",
    };
    static constexpr std::array<std::string_view, 2> codesThunder{
        "lighting",
        "lightning-rainy",
    };
    static constexpr std::array<std::string_view, 2> codesWind{
        "windy",
        "windy-variant",
    };

    static constexpr std::array<WeatherHandler::Codeset, 9> codesets{{
        {WeatherHandler::Conditions::CLEAR, codesClear},
        {WeatherHandler::Conditions::CLOUDY, codesCloudy},
        {WeatherHandler::Conditions::CLOUDY_PARTLY, codesCloudyPartly},
        {WeatherHandler::Conditions::EXCEPTION, codesException},
        {WeatherHandler::Conditions::FOG, codesFog},
        {WeatherHandler::Conditions::RAIN, codesRain},
        {WeatherHandler::Conditions::SNOW, codesSnow},
        {WeatherHandler::Conditions::THUNDER, codesThunder},
        {WeatherHandler::Conditions::WIND, codesWind},
    }};

    // https://developers.home-assistant.io/docs/api/rest
    inline static std::vector<const char *> paths{
        "/api/states/weather.forecast_home",
#ifdef HOMEASSISTANT_ENTITY
        "/api/states/" HOMEASSISTANT_ENTITY,
#endif
    };

public:
    static constexpr std::string_view name{"Home Assistant"};

    explicit HomeAssistantWeatherMiddleware() : WeatherHandler(name, 1U << 18U)
    {
#ifdef HOMEASSISTANT_HOST
        host = HOMEASSISTANT_HOST;
#else
        host = "homeassistant.local";
#endif // HOMEASSISTANT_HOST
#ifdef HOMEASSISTANT_PORT
        port = HOMEASSISTANT_PORT;
#else
        port = 8123;
#endif // HOMEASSISTANT_PORT
#ifdef HOMEASSISTANT_PROTOCOL
        tls = strcmp(HOMEASSISTANT_PROTOCOL, "http:");
#else
        tls = false;
#endif // HOMEASSISTANT_PROTOCOL
        headers["Authorization"] = "Bearer " HOMEASSISTANT_KEY;
    };

    void update(std::optional<WeatherHandler::Conditions> &condition, std::optional<int16_t> &temperature,
                unsigned long &lastMillis) override;
};

#endif // WEATHER_HOMEASSISTANT
