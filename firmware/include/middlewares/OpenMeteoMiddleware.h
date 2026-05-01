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
    static constexpr std::array<uint8_t, 1> codesClear{0};
    static constexpr std::array<uint8_t, 1> codesCloudy{3};
    static constexpr std::array<uint8_t, 2> codesCloudyPartly{1, 2};
    static constexpr std::array<uint8_t, 2> codesFog{45, 48};
    static constexpr std::array<uint8_t, 13> codesRain{51, 53, 55, 56, 57, 61, 63, 65, 66, 67, 80, 81, 82};
    static constexpr std::array<uint8_t, 6> codesSnow{71, 73, 75, 77, 85, 86};
    static constexpr std::array<uint8_t, 3> codesThunder{95, 96, 99};

    static constexpr std::array<WeatherHandler::Codeset8, 7> codesets{{
        {WeatherHandler::Conditions::CLEAR, codesClear},
        {WeatherHandler::Conditions::CLOUDY, codesCloudy},
        {WeatherHandler::Conditions::CLOUDY_PARTLY, codesCloudyPartly},
        {WeatherHandler::Conditions::FOG, codesFog},
        {WeatherHandler::Conditions::RAIN, codesRain},
        {WeatherHandler::Conditions::SNOW, codesSnow},
        {WeatherHandler::Conditions::THUNDER, codesThunder},
    }};

    // https://open-meteo.com/en/docs#api-documentation
    static inline std::vector<std::pair<const char *, const char *>> parts{
        {
            "api.open-meteo.com",
            "latitude=" LATITUDE "&longitude=" LONGITUDE "&current=temperature_2m,weather_code",
        },
#if TEMPERATURE_CELSIUS
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
#endif // TEMPERATURE_CELSIUS
#ifdef OPENMETEO_KEY
        {
            "customer-api.open-meteo.com",
            "latitude=" LATITUDE "&longitude=" LONGITUDE "&current=temperature_2m,weather_code&apikey=" OPENMETEO_KEY,
        },
#endif // OPENMETEO_KEY
#if defined(OPENMETEO_KEY) && TEMPERATURE_CELSIUS
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
#endif // defined(OPENMETEO_KEY) && TEMPERATURE_CELSIUS
    };

public:
    static constexpr std::string_view name{"Open-Meteo"};

    explicit OpenMeteoMiddleware() : WeatherHandler(name) { path = "/v1/forecast"; };

    void update(std::optional<WeatherHandler::Conditions> &condition, std::optional<int16_t> &temperature,
                unsigned long &lastMillis) override;
};

#endif // WEATHER_OPENMETEO
