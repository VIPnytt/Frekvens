#if WEATHER_OPENMETEO

#include "middlewares/OpenMeteoMiddleware.h"

#include <ArduinoJson.h> // NOLINT(misc-include-cleaner)

void OpenMeteoMiddleware::update(std::optional<WeatherHandler::Conditions> &condition,
                                 std::optional<int16_t> &temperature, unsigned long &lastMillis)
{
    if (parts.empty())
    {
        ESP_LOGE("Weather", "provider unavailable"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        return;
    }
    host = parts.back().first;
    query = parts.back().second;
    std::vector<char> body;
    const int status = fetch(body, lastMillis);
    if (status != 200)
    {
        if (status >= 400 && status < 500)
        {
            parts.pop_back();
            lastMillis = millis() - interval + (1U << 12U);
        }
        return;
    }
    JsonDocument filter; // NOLINT(misc-const-correctness)
    filter["current"]["temperature_2m"].set(true);
    filter["current"]["weather_code"].set(true);
    JsonDocument doc; // NOLINT(misc-const-correctness)
    if (deserializeJson(doc, body.data(), DeserializationOption::Filter(filter)) == DeserializationError::Code::Ok &&
        doc["current"]["temperature_2m"].is<float>() && doc["current"]["weather_code"].is<uint8_t>())
    {
        condition = getCondition(doc["current"]["weather_code"].as<uint8_t>(), codesets);
        temperature = static_cast<int16_t>(roundf(doc["current"]["temperature_2m"].as<float>()));
        return;
    }
    parts.pop_back();
    ESP_LOGD("Response", "unsupported format"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    lastMillis = millis() - interval + (1U << 13U);
}

#endif // WEATHER_OPENMETEO
