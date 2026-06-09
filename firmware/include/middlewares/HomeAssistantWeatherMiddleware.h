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
    static constexpr std::array<std::string_view, 2U> codesClear{
        "clear-night",
        "sunny",
    };
    static constexpr std::array<std::string_view, 1U> codesCloudy{
        "cloudy",
    };
    static constexpr std::array<std::string_view, 1U> codesCloudyPartly{
        "partlycloudy",
    };
    static constexpr std::array<std::string_view, 1U> codesException{
        "exceptional",
    };
    static constexpr std::array<std::string_view, 1U> codesFog{
        "fog",
    };
    static constexpr std::array<std::string_view, 2U> codesRain{
        "pouring",
        "rainy",
    };
    static constexpr std::array<std::string_view, 3U> codesSnow{
        "hail",
        "snowy",
        "snowy-rainy",
    };
    static constexpr std::array<std::string_view, 2U> codesThunder{
        "lighting",
        "lightning-rainy",
    };
    static constexpr std::array<std::string_view, 2U> codesWind{
        "windy",
        "windy-variant",
    };

    static constexpr std::array<std::pair<WeatherHandler::Condition, std::span<const std::string_view>>, 9> codesets{{
        {WeatherHandler::Condition::CLEAR, codesClear},
        {WeatherHandler::Condition::CLOUDY, codesCloudy},
        {WeatherHandler::Condition::CLOUDY_PARTLY, codesCloudyPartly},
        {WeatherHandler::Condition::EXCEPTION, codesException},
        {WeatherHandler::Condition::FOG, codesFog},
        {WeatherHandler::Condition::RAIN, codesRain},
        {WeatherHandler::Condition::SNOW, codesSnow},
        {WeatherHandler::Condition::THUNDER, codesThunder},
        {WeatherHandler::Condition::WIND, codesWind},
    }};

    // https://developers.home-assistant.io/docs/api/rest
    static inline std::vector<const char *> paths{
        "/api/states/weather.forecast_home",
#ifdef HOMEASSISTANT_ENTITY
        "/api/states/" HOMEASSISTANT_ENTITY,
#endif
    };

public:
    static constexpr std::string_view name{"Home Assistant"};

    explicit HomeAssistantWeatherMiddleware() : WeatherHandler(name, 0b1U << 18U)
    {
#ifdef HOMEASSISTANT_HOST
        host = HOMEASSISTANT_HOST;
#else
        host = "homeassistant.local";
#endif // HOMEASSISTANT_HOST
#ifdef HOMEASSISTANT_PORT
        port = HOMEASSISTANT_PORT;
#else
        port = 8123U;
#endif // HOMEASSISTANT_PORT
#ifdef HOMEASSISTANT_PROTOCOL
        tls = strcmp(HOMEASSISTANT_PROTOCOL, "http:") != 0;
#else
        tls = false;
#endif // HOMEASSISTANT_PROTOCOL
        headers.push_back({"Authorization", "Bearer " HOMEASSISTANT_KEY});
    };

    void update(std::optional<WeatherHandler::Condition> &condition, std::optional<int16_t> &temperature,
                unsigned long &lastMillis) override;
};

#endif // WEATHER_HOMEASSISTANT
