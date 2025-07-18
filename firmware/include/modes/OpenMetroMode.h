#pragma once

#include "config/constants.h"

#if defined(LATITUDE) && defined(LONGITUDE)

#include <vector>

#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

class OpenMetroMode : public ModeModule
{
private:
    unsigned long lastMillis = 0;

    // https://open-meteo.com/en/docs#api-documentation
    std::vector<const char *> urls = {
        "https://api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE "&current=temperature_2m,weather_code",
#if defined(OPENMETRO_PARAMETERS)
        "https://api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE "&current=temperature_2m,weather_code&" OPENMETRO_PARAMETERS,
#endif
#if defined(OPENMETRO_KEY)
        "https://customer-api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE "&current=temperature_2m,weather_code&apikey=" OPENMETRO_KEY,
#endif
#if defined(OPENMETRO_KEY) && defined(OPENMETRO_PARAMETERS)
        "https://customer-api.open-meteo.com/v1/forecast?latitude=" LATITUDE "&longitude=" LONGITUDE "&current=temperature_2m,weather_code&apikey=" OPENMETRO_KEY "&" OPENMETRO_PARAMETERS,
#endif
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
    OpenMetroMode() : ModeModule("Open-Metro") {};

#if EXTENSION_BUILD && (defined(OPENMETRO_KEY) || defined(OPENMETRO_PARAMETERS))
    void setup() override;
#endif
    void wake() override;
    void handle() override;
};

#endif // defined(LATITUDE) && defined(LONGITUDE)
