#pragma once

#if MODE_OPENWEATHER

#include "config/constants.h"
#include "handlers/WeatherHandler.h"
#include "modules/ModeModule.h"

#include <vector>

class OpenWeatherMode final : public ModeModule
{
private:
    static constexpr uint32_t interval = 600'000; // Recommended update interval: 10 minutes

    unsigned long lastMillis = 0;

    // https://openweathermap.org/api/one-call-3#current
    // https://openweathermap.org/current#one
    std::vector<const char *> urls{
        "https://api.openweathermap.org/data/3.0/onecall?lat=" LATITUDE "&lon=" LONGITUDE
        "&exclude=alerts,daily,hourly,minutely&appid=" OPENWEATHER_KEY,
        "https://api.openweathermap.org/data/2.5/weather?lat=" LATITUDE "&lon=" LONGITUDE "&appid=" OPENWEATHER_KEY,
#if TEMPERATURE_KELVIN
        "https://api.openweathermap.org/data/3.0/onecall?lat=" LATITUDE "&lon=" LONGITUDE
        "&exclude=alerts,daily,hourly,minutely&units=standard&appid=" OPENWEATHER_KEY,
        "https://api.openweathermap.org/data/2.5/weather?lat=" LATITUDE "&lon=" LONGITUDE
        "&units=standard&appid=" OPENWEATHER_KEY,
#elif TEMPERATURE_CELSIUS
        "https://api.openweathermap.org/data/3.0/onecall?lat=" LATITUDE "&lon=" LONGITUDE
        "&exclude=alerts,daily,hourly,minutely&units=metric&appid=" OPENWEATHER_KEY,
        "https://api.openweathermap.org/data/2.5/weather?lat=" LATITUDE "&lon=" LONGITUDE
        "&units=metric&appid=" OPENWEATHER_KEY,
#elif TEMPERATURE_FAHRENHEIT
        "https://api.openweathermap.org/data/3.0/onecall?lat=" LATITUDE "&lon=" LONGITUDE
        "&exclude=alerts,daily,hourly,minutely&units=imperial&appid=" OPENWEATHER_KEY,
        "https://api.openweathermap.org/data/2.5/weather?lat=" LATITUDE "&lon=" LONGITUDE
        "&units=imperial&appid=" OPENWEATHER_KEY,
#endif // TEMPERATURE_KELVIN
    };

    // https://openweathermap.org/weather-conditions#Weather-Condition-Codes-2
    static inline constexpr std::array<uint16_t, 1> codesClear{800};
    static inline constexpr std::array<uint16_t, 1> codesCloudy{804};
    static inline constexpr std::array<uint16_t, 3> codesCloudyPartly{801, 802, 803};
    static inline constexpr std::array<uint16_t, 10> codesFog{701, 711, 721, 731, 741, 751, 761, 762, 771, 781};
    static inline constexpr std::array<uint16_t, 19> codesRain{
        300, 301, 302, 310, 311, 312, 313, 314, 321, 500, 501, 502, 503, 504, 511, 520, 521, 522, 531};
    static inline constexpr std::array<uint16_t, 11> codesSnow{600, 601, 602, 611, 612, 613, 615, 616, 620, 621, 622};
    static inline constexpr std::array<uint16_t, 10> codesThunder{200, 201, 202, 210, 211, 212, 221, 230, 231, 232};

    static inline constexpr std::array<WeatherHandler::Codeset16, 7> codesets{{
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
    explicit OpenWeatherMode() : ModeModule("OpenWeather") {};

    void begin() override;
    void handle() override;
};

#endif // MODE_OPENWEATHER
