#pragma once

#if MODE_WEATHER

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "handlers/WeatherHandler.h"
#include "middlewares/GoogleWeatherMiddleware.h"
#include "middlewares/HomeAssistantWeatherMiddleware.h"
#include "middlewares/OpenMeteoMiddleware.h"
#include "middlewares/OpenWeatherMiddleware.h"
#include "middlewares/WorldWeatherOnlineMiddleware.h"
#include "middlewares/WttrInMiddleware.h"
#include "middlewares/YrMiddleware.h"
#include "modules/ModeModule.h"

#include <array>
#include <memory>
#include <optional>
#include <vector>

class WeatherMode final : public ModeModule
{
private:
    static constexpr std::array<std::string_view, 9> conditionNames{
        "Clear",
        "Cloudy",
        "Partly cloudy",
        "Exception",
        "Fog",
        "Rain",
        "Snow",
        "Thunder",
        "Wind",
    };

    static constexpr std::array<std::string_view, COUNT_WEATHER> providerNames{
#if WEATHER_GOOGLEWEATHER
        GoogleWeatherMiddleware::name,
#endif
#if WEATHER_HOMEASSISTANTWEATHER
        HomeAssistantWeatherMiddleware::name,
#endif
#if WEATHER_OPENMETEO
        OpenMeteoMiddleware::name,
#endif
#if WEATHER_OPENWEATHER
        OpenWeatherMiddleware::name,
#endif
#if WEATHER_WORLDWEATHERONLINE
        WorldWeatherOnlineMiddleware::name,
#endif
#if WEATHER_WTTRIN
        WttrInMiddleware::name,
#endif
#if WEATHER_YR
        YrMiddleware::name,
#endif
    };

    unsigned long lastMillis = 0;

    std::optional<WeatherHandler::Conditions> condition;

    std::optional<int16_t> temperature;

    std::unique_ptr<WeatherHandler> provider;

    void setProvider(std::string_view providerName);

    void transmit();

public:
    explicit WeatherMode() : ModeModule("Weather") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void end() override;

    void onReceive(JsonObjectConst payload, const char *source) override;
};

#endif // MODE_WEATHER
