#pragma once

#if MODE_OPENMETEO

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

#include <vector>

class OpenMeteoMode final : public ModeModule
{
private:
    static constexpr uint32_t interval = 900'000; // Data resolution: down to 15 minutes (depending on location)

    unsigned long lastMillis = 0;

    // https://open-meteo.com/en/docs#api-documentation
    std::vector<const char *> urls{
        "https://api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE
        "&current=temperature_2m,weather_code",
#if TEMPERATURE_CELSIUS
        "https://api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE
        "&current=temperature_2m,weather_code&temperature_unit=celsius",
#elif TEMPERATURE_FAHRENHEIT
        "https://api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE
        "&current=temperature_2m,weather_code&temperature_unit=fahrenheit",
#endif // TEMPERATURE_CELSIUS
#ifdef OPENMETEO_KEY
        "https://customer-api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE
        "&current=temperature_2m,weather_code&apikey=" OPENMETEO_KEY,
#endif // OPENMETEO_KEY
#if defined(OPENMETEO_KEY) && TEMPERATURE_CELSIUS
        "https://customer-api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE
        "&current=temperature_2m,weather_code&temperature_unit=celsius&apikey=" OPENMETEO_KEY,
#elif defined(OPENMETEO_KEY) && TEMPERATURE_FAHRENHEIT
        "https://customer-api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE
        "&current=temperature_2m,weather_code&temperature_unit=fahrenheit&apikey=" OPENMETEO_KEY,
#endif // defined(OPENMETEO_KEY) && TEMPERATURE_CELSIUS

    };

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

    void update();

public:
    explicit OpenMeteoMode() : ModeModule("Open-Meteo") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_OPENMETEO
