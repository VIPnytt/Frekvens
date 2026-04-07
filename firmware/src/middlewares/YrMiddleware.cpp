#if WEATHER_YR

#include "middlewares/YrMiddleware.h"

#include <ArduinoJson.h> // NOLINT(misc-include-cleaner)

void YrMiddleware::update(std::optional<WeatherHandler::Conditions> &condition, std::optional<int16_t> &temperature,
                          unsigned long &lastMillis)
{
    if (paths.empty())
    {
        ESP_LOGE(name, "weather provider unavailable"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        return;
    }
    path = paths.back();
    std::vector<char> body;
    const int status = fetch(body, lastMillis);
    if (status != 200)
    {
        if (status >= 400 && status < 500)
        {
            paths.pop_back();
            lastMillis = millis() - interval + (1U << 12U);
        }
        return;
    }
    JsonDocument filter; // NOLINT(misc-const-correctness)
    filter["properties"]["timeseries"][0]["data"]["instant"]["details"]["air_temperature"].set(true);
    filter["properties"]["timeseries"][0]["data"]["next_1_hours"]["summary"]["symbol_code"].set(true);
    JsonDocument doc; // NOLINT(misc-const-correctness)
    if (deserializeJson(doc, body.data(), DeserializationOption::Filter(filter)) == DeserializationError::Code::Ok &&
        doc["properties"]["timeseries"][0]["data"]["instant"]["details"]["air_temperature"].is<float>() &&
        doc["properties"]["timeseries"][0]["data"]["next_1_hours"]["summary"]["symbol_code"].is<std::string_view>())
    {
        condition = getCondition(
            doc["properties"]["timeseries"][0]["data"]["next_1_hours"]["summary"]["symbol_code"].as<std::string_view>(),
            codesets);
        temperature = static_cast<int16_t>(
            roundf(doc["properties"]["timeseries"][0]["data"]["instant"]["details"]["air_temperature"].as<float>()));
        return;
    }
    paths.pop_back();
    ESP_LOGD(name, "unsupported format"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    lastMillis = millis() - interval + (1U << 13U);
}

#endif // WEATHER_YR
