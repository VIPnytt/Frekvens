#pragma once

#if MODE_HOMEASSISTANTWEATHER

#include "config/constants.h"
#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

#include <vector>

class HomeAssistantWeatherMode : public ModeModule
{
private:
    static constexpr uint16_t interval = UINT16_MAX;

#ifdef HOMEASSISTANT_PROTOCOL
    static constexpr std::string_view protocol = HOMEASSISTANT_PROTOCOL;
#else
    static constexpr std::string_view protocol = "http:";
#endif
#ifdef HOMEASSISTANT_HOST
    static constexpr std::string_view host = HOMEASSISTANT_HOST;
#else
    static constexpr std::string_view host = "homeassistant.local";
#endif
#ifdef HOMEASSISTANT_PORT
    static constexpr uint16_t port = HOMEASSISTANT_PORT;
#else
    static constexpr uint16_t port = 8123;
#endif

    unsigned long lastMillis = 0;

    // https://developers.home-assistant.io/docs/api/rest
    std::vector<std::string> urls = {
        std::string(protocol)
            .append("//")
            .append(host)
            .append(":")
            .append(std::to_string(port))
            .append("/api/states/weather.forecast_home"),
#ifdef HOMEASSISTANT_ENTITY
        std::string(protocol)
            .append("//")
            .append(host)
            .append(":")
            .append(std::to_string(port))
            .append("/api/states/" HOMEASSISTANT_ENTITY),
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

    void begin() override;
    void handle() override;
};

#endif // MODE_HOMEASSISTANTWEATHER
