#if WEATHER_YR

#include "middlewares/YrMiddleware.h"

#include <ArduinoJson.h> // NOLINT(misc-include-cleaner)

void YrMiddleware::update(std::optional<WeatherHandler::Conditions> &condition, std::optional<int16_t> &temperature,
                          unsigned long &lastMillis)
{
    if (paths.empty())
    {
        ESP_LOGE("Weather", "provider unavailable"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        return;
    }
    path = paths.back();
    std::vector<char> body;
    const int status{fetch(body, lastMillis)};
    if (status != 200)
    {
        if (status >= 400 && status < 500)
        {
            paths.pop_back();
            lastMillis = millis() - interval + (0b1U << 12U);
        }
        return;
    }
    JsonDocument filter; // NOLINT(misc-const-correctness)
    filter["properties"]["timeseries"][0U]["data"]["instant"]["details"]["air_temperature"].set(true);
    filter["properties"]["timeseries"][0U]["data"]["next_1_hours"]["summary"]["symbol_code"].set(true);
    JsonDocument doc; // NOLINT(misc-const-correctness)
    if (deserializeJson(doc, body.data(), DeserializationOption::Filter(filter)) == DeserializationError::Code::Ok &&
        doc["properties"]["timeseries"][0U]["data"]["instant"]["details"]["air_temperature"].is<float>() &&
        doc["properties"]["timeseries"][0U]["data"]["next_1_hours"]["summary"]["symbol_code"].is<std::string_view>())
    {
        condition = getCondition(doc["properties"]["timeseries"][0U]["data"]["next_1_hours"]["summary"]["symbol_code"]
                                     .as<std::string_view>(),
                                 codesets);
#if TEMPERATURE_FAHRENHEIT
        temperature = static_cast<int16_t>(
            lroundf(doc["properties"]["timeseries"][0U]["data"]["instant"]["details"]["air_temperature"].as<float>() *
                    9.0F / 5.0F) +
            32);
#elif TEMPERATURE_KELVIN
        temperature = static_cast<int16_t>(
            lroundf(273.15F +
                    doc["properties"]["timeseries"][0U]["data"]["instant"]["details"]["air_temperature"].as<float>()));
#else
        temperature = static_cast<int16_t>(
            lroundf(doc["properties"]["timeseries"][0U]["data"]["instant"]["details"]["air_temperature"].as<float>()));
#endif // TEMPERATURE_FAHRENHEIT
        return;
    }
    paths.pop_back();
    ESP_LOGD("Response", "unsupported format"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    lastMillis = millis() - interval + (0b1U << 13U);
}

#endif // WEATHER_YR
