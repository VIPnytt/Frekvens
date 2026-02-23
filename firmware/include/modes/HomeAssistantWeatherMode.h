#pragma once

#if MODE_HOMEASSISTANTWEATHER

#include "config/constants.h"
#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

#include <vector>

class HomeAssistantWeatherMode final : public ModeModule
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
    std::vector<std::string> urls{
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

    void update();

public:
    explicit HomeAssistantWeatherMode() : ModeModule("Home Assistant weather") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_HOMEASSISTANTWEATHER
