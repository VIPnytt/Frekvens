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
    static constexpr std::array<uint16_t, 1U> codesClear{113U};
    static constexpr std::array<uint16_t, 2U> codesCloudy{119U, 122U};
    static constexpr std::array<uint16_t, 1U> codesCloudyPartly{116U};
    static constexpr std::array<uint16_t, 3U> codesFog{143U, 248U, 260U};
    static constexpr std::array<uint16_t, 19U> codesRain{
        176U,
        293U,
        296U,
        299U,
        302U,
        305U,
        308U,
        311U,
        314U,
        353U,
        356U,
        359U,
        386U,
        389U,
        263U,
        266U,
        281U,
        284U,
        185U,
    };
    static constexpr std::array<uint16_t, 14U> codesSnow{
        179U, 227U, 323U, 326U, 329U, 332U, 335U, 338U, 368U, 371U, 392U, 395U, 230U, 350U};
    static constexpr std::array<uint16_t, 5U> codesThunder{200U, 386U, 389U, 392U, 395U};

    static constexpr std::array<std::pair<Condition, std::span<const uint16_t>>, 7U> codesets{{
        {Condition::CLEAR, codesClear},
        {Condition::CLOUDY, codesCloudy},
        {Condition::CLOUDY_PARTLY, codesCloudyPartly},
        {Condition::FOG, codesFog},
        {Condition::RAIN, codesRain},
        {Condition::SNOW, codesSnow},
        {Condition::THUNDER, codesThunder},
    }};

    // https://wttr.in/:help
    // https://github.com/chubin/wttr.in#readme
    static inline std::vector<std::pair<const char *, const char *>> parts{
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

    explicit WttrInMiddleware() : WeatherHandler(name) { host = "wttr.in"; };

    void update(std::optional<WeatherHandler::Condition> &condition, std::optional<int16_t> &temperature,
                unsigned long &lastMillis) override;
};

#endif // WEATHER_WTTRIN
