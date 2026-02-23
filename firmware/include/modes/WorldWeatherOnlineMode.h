#pragma once

#if MODE_WORLDWEATHERONLINE

#include "config/constants.h"
#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

#include <vector>

class WorldWeatherOnlineMode final : public ModeModule
{
private:
    static constexpr uint32_t interval = 600'000; // Recommended update interval: 10-15 minutes

    unsigned long lastMillis = 0;

    // https://www.worldweatheronline.com/weather-api/api/docs/local-city-town-weather-api.aspx
    std::vector<const char *> urls{
#ifdef LOCATION
        "https://api.worldweatheronline.com/premium/v1/weather.ashx?q=" LOCATION
        "&cc=yes&fx=no&mca=no&format=json&key=" WORLDWEATHERONLINE_KEY,
#endif
#if defined(LATITUDE) && defined(LONGITUDE)
        "https://api.worldweatheronline.com/premium/v1/weather.ashx?q=" LATITUDE "," LONGITUDE
        "&cc=yes&fx=no&mca=no&format=json&key=" WORLDWEATHERONLINE_KEY,
#endif
    };

    // https://www.worldweatheronline.com/weather-api/api/docs/weather-icons.aspx
    static constexpr std::array<uint16_t, 1> codesClear{113};
    static constexpr std::array<uint16_t, 2> codesCloudy{119, 122};
    static constexpr std::array<uint16_t, 1> codesCloudyPartly{116};
    static constexpr std::array<uint16_t, 3> codesFog{143, 248, 260};
    static constexpr std::array<uint16_t, 9> codesRain{176, 263, 266, 293, 296, 299, 302, 305, 308};
    static constexpr std::array<uint16_t, 11> codesSnow{179, 182, 185, 227, 230, 281, 284, 311, 314, 317, 320};
    static constexpr std::array<uint16_t, 1> codesThunder{200};

    static constexpr std::array<WeatherHandler::Codeset16, 7> codesets{{
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
    explicit WorldWeatherOnlineMode() : ModeModule("World Weather Online") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_WORLDWEATHERONLINE
