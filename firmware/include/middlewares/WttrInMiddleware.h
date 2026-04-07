#pragma once

#if WEATHER_WTTRIN

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "handlers/WeatherHandler.h"

#include <array>
#include <string_view>
#include <vector>

class WttrInMiddleware final : public WeatherHandler
{
private:
    // https://github.com/chubin/wttr.in/blob/master/lib/constants.py
    static constexpr std::array<uint16_t, 1> codesClear{113};
    static constexpr std::array<uint16_t, 2> codesCloudy{119, 122};
    static constexpr std::array<uint16_t, 1> codesCloudyPartly{116};
    static constexpr std::array<uint16_t, 3> codesFog{143, 248, 260};
    static constexpr std::array<uint16_t, 19> codesRain{
        176, 293, 296, 299, 302, 305, 308, 311, 314, 353, 356, 359, 386, 389, 263, 266, 281, 284, 185};
    static constexpr std::array<uint16_t, 14> codesSnow{
        179, 227, 323, 326, 329, 332, 335, 338, 368, 371, 392, 395, 230, 350};
    static constexpr std::array<uint16_t, 5> codesThunder{200, 386, 389, 392, 395};

    static constexpr std::array<Codeset16, 7> codesets{{
        {Conditions::CLEAR, codesClear},
        {Conditions::CLOUDY, codesCloudy},
        {Conditions::CLOUDY_PARTLY, codesCloudyPartly},
        {Conditions::FOG, codesFog},
        {Conditions::RAIN, codesRain},
        {Conditions::SNOW, codesSnow},
        {Conditions::THUNDER, codesThunder},
    }};

    // https://wttr.in/:help
    // https://github.com/chubin/wttr.in#readme
    inline static std::vector<std::pair<const char *, const char *>> parts{
        {
            "/",
            "format=j1",
        },
        {
            "/",
            "format=j2",
        },
#ifdef LOCATION
        {
            "/" LOCATION,
            "format=j1",
        },
        {
            "/" LOCATION,
            "format=j2",
        },
#endif
#if defined(LATITUDE) && defined(LONGITUDE)
        {
            "/" LATITUDE "," LONGITUDE,
            "format=j1",
        },
        {
            "/" LATITUDE "," LONGITUDE,
            "format=j2",
        },
#endif
    };

public:
    static constexpr std::string_view name{"Wttr.in"};

    explicit WttrInMiddleware() : WeatherHandler(name.data()) { host = "wttr.in"; };

    void update(std::optional<WeatherHandler::Conditions> &condition, std::optional<int16_t> &temperature,
                unsigned long &lastMillis) override;
};

#endif // WEATHER_WTTRIN
