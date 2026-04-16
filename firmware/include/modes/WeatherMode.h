#pragma once

#if MODE_WEATHER

#include "config/constants.h"                           // NOLINT(misc-include-cleaner)
#include "middlewares/GoogleWeatherMiddleware.h"        // NOLINT(misc-include-cleaner)
#include "middlewares/HomeAssistantWeatherMiddleware.h" // NOLINT(misc-include-cleaner)
#include "middlewares/OpenMeteoMiddleware.h"            // NOLINT(misc-include-cleaner)
#include "middlewares/OpenWeatherMiddleware.h"          // NOLINT(misc-include-cleaner)
#include "middlewares/WorldWeatherOnlineMiddleware.h"   // NOLINT(misc-include-cleaner)
#include "middlewares/WttrInMiddleware.h"               // NOLINT(misc-include-cleaner)
#include "middlewares/YrMiddleware.h"                   // NOLINT(misc-include-cleaner)
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

    static constexpr auto providerNames{std::to_array<std::string_view>({
#if WEATHER_GOOGLE
        GoogleWeatherMiddleware::name,
#endif
#if WEATHER_HOMEASSISTANT
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
    })};

    unsigned long lastMillis = 0;

    std::optional<WeatherHandler::Conditions> condition{};

    std::optional<int16_t> temperature{};

    std::unique_ptr<WeatherHandler> provider{};

    void setProvider(std::string_view providerName);

    void transmit();

public:
    static constexpr std::string_view name{"Weather"};

    explicit WeatherMode() : ModeModule(name) {};

    void configure() override;
    void begin() override;
    void handle() override;

    void onReceive(JsonObjectConst payload, std::string_view source) override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif
};

#endif // MODE_WEATHER
