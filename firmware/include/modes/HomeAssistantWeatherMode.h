#pragma once

#if MODE_HOMEASSISTANTWEATHER

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

#include <vector>

class HomeAssistantWeatherMode final : public ModeModule
{
private:
    static constexpr uint16_t interval = UINT16_MAX;

    unsigned long lastMillis = 0;

    // https://developers.home-assistant.io/docs/api/rest
    std::vector<const char *> paths{
        "/api/states/weather.forecast_home",
#ifdef HOMEASSISTANT_ENTITY
        "/api/states/" HOMEASSISTANT_ENTITY,
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
