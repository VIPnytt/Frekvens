#pragma once

#include "config/constants.h"

#if MODE_OPENMETEO

#include <vector>

#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

class OpenMeteoMode : public ModeModule
{
private:
        static constexpr uint32_t interval = 900'000; // Data resolution: down to 15 minutes (depending on location)

    unsigned long lastMillis = 0;

    // https://open-meteo.com/en/docs#api-documentation
    std::vector<const char *> urls = {
        "https://api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE "&current=temperature_2m,weather_code",
#if TEMPERATURE_CELSIUS
        "https://api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE "&current=temperature_2m,weather_code&temperature_unit=celsius",
#elif TEMPERATURE_FAHRENHEIT
        "https://api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE "&current=temperature_2m,weather_code&temperature_unit=fahrenheit",
#endif // TEMPERATURE_CELSIUS
#ifdef OPENMETEO_KEY
        "https://customer-api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE "&current=temperature_2m,weather_code&apikey=" OPENMETEO_KEY,
#endif // OPENMETEO_KEY
#if defined(OPENMETEO_KEY) && TEMPERATURE_CELSIUS
        "https://customer-api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE "&current=temperature_2m,weather_code&temperature_unit=celsius&apikey=" OPENMETEO_KEY,
#elif defined(OPENMETEO_KEY) && TEMPERATURE_FAHRENHEIT
        "https://customer-api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE "&current=temperature_2m,weather_code&temperature_unit=fahrenheit&apikey=" OPENMETEO_KEY,
#endif // defined(OPENMETEO_KEY) && TEMPERATURE_CELSIUS

    };

    // https://open-meteo.com/en/docs#weather_variable_documentation
    const std::vector<WeatherHandler::Codeset8> codesets = {
        {
            WeatherHandler::Conditions::CLEAR,
            {0},
        },
        {
            WeatherHandler::Conditions::CLOUDY,
            {3},
        },
        {
            WeatherHandler::Conditions::CLOUDY_PARTLY,
            {1, 2},
        },
        {
            WeatherHandler::Conditions::FOG,
            {45, 48},
        },
        {
            WeatherHandler::Conditions::RAIN,
            {51, 53, 55, 56, 57, 61, 63, 65, 66, 67, 80, 81, 82},
        },
        {
            WeatherHandler::Conditions::SNOW,
            {71, 73, 75, 77, 85, 86},
        },
        {
            WeatherHandler::Conditions::THUNDER,
            {95, 96, 99},
        },
    };

    void update();

public:
    OpenMeteoMode() : ModeModule("Open-Meteo") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_OPENMETEO
