#pragma once

#if WEATHER_OPENMETEO

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "handlers/WeatherHandler.h"

#include <array>
#include <string_view>
#include <vector>

class OpenMeteoMiddleware final : public WeatherHandler
{
private:
    // https://open-meteo.com/en/docs#weather_variable_documentation
    static constexpr std::array<uint8_t, 1U> codesClear{0U};
    static constexpr std::array<uint8_t, 1U> codesCloudy{3U};
    static constexpr std::array<uint8_t, 2U> codesCloudyPartly{1U, 2U};
    static constexpr std::array<uint8_t, 2U> codesFog{45U, 48U};
    static constexpr std::array<uint8_t, 13U> codesRain{
        51U, 53U, 55U, 56U, 57U, 61U, 63U, 65U, 66U, 67U, 80U, 81U, 82U};
    static constexpr std::array<uint8_t, 6U> codesSnow{71U, 73U, 75U, 77U, 85U, 86U};
    static constexpr std::array<uint8_t, 3U> codesThunder{95U, 96U, 99U};

    static constexpr std::array<std::pair<Condition, std::span<const uint8_t>>, 7U> codesets{{
        {WeatherHandler::Condition::CLEAR, codesClear},
        {WeatherHandler::Condition::CLOUDY, codesCloudy},
        {WeatherHandler::Condition::CLOUDY_PARTLY, codesCloudyPartly},
        {WeatherHandler::Condition::FOG, codesFog},
        {WeatherHandler::Condition::RAIN, codesRain},
        {WeatherHandler::Condition::SNOW, codesSnow},
        {WeatherHandler::Condition::THUNDER, codesThunder},
    }};

    // https://open-meteo.com/en/docs#api-documentation
    static inline std::vector<std::pair<const char *, const char *>> parts{
#if TEMPERATURE_CELSIUS || TEMPERATURE_KELVIN
        {
            "api.open-meteo.com",
            "latitude=" LATITUDE "&longitude=" LONGITUDE
            "&current=temperature_2m,weather_code&temperature_unit=celsius",
        },
#elif TEMPERATURE_FAHRENHEIT
        {
            "api.open-meteo.com",
            "latitude=" LATITUDE "&longitude=" LONGITUDE
            "&current=temperature_2m,weather_code&temperature_unit=fahrenheit",
        },
#else
        {
            "api.open-meteo.com",
            "latitude=" LATITUDE "&longitude=" LONGITUDE "&current=temperature_2m,weather_code",
        },
#endif // TEMPERATURE_CELSIUS || TEMPERATURE_KELVIN
#if defined(OPENMETEO_KEY) && (TEMPERATURE_CELSIUS || TEMPERATURE_KELVIN)
        {
            "customer-api.open-meteo.com",
            "latitude=" LATITUDE "&longitude=" LONGITUDE
            "&current=temperature_2m,weather_code&temperature_unit=celsius&apikey=" OPENMETEO_KEY,
        },
#elif defined(OPENMETEO_KEY) && TEMPERATURE_FAHRENHEIT
        {
            "customer-api.open-meteo.com",
            "latitude=" LATITUDE "&longitude=" LONGITUDE
            "&current=temperature_2m,weather_code&temperature_unit=fahrenheit&apikey=" OPENMETEO_KEY,
        },
#elif defined(OPENMETEO_KEY)
        {
            "customer-api.open-meteo.com",
            "latitude=" LATITUDE "&longitude=" LONGITUDE "&current=temperature_2m,weather_code&apikey=" OPENMETEO_KEY,
        },
#endif // defined(OPENMETEO_KEY) && (TEMPERATURE_CELSIUS || TEMPERATURE_KELVIN)
    };

public:
    static constexpr std::string_view name{"Open-Meteo"};

    explicit OpenMeteoMiddleware() : WeatherHandler(name) { path = "/v1/forecast"; };

    void update(std::optional<WeatherHandler::Condition> &condition, std::optional<int16_t> &temperature,
                unsigned long &lastMillis) override;
};

#endif // WEATHER_OPENMETEO
