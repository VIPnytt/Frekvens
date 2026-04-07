#if WEATHER_OPENWEATHER

#include "middlewares/OpenWeatherMiddleware.h"

#include <ArduinoJson.h> // NOLINT(misc-include-cleaner)

void OpenWeatherMiddleware::update(std::optional<WeatherHandler::Conditions> &condition,
                                   std::optional<int16_t> &temperature, unsigned long &lastMillis)
{
    if (parts.empty())
    {
        ESP_LOGE(name, "weather provider unavailable"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        return;
    }
    path = parts.back().first;
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
    filter["current"]["temp"].set(true);
    filter["current"]["weather"]["id"].set(true);
    filter["main"]["temp"].set(true);
    filter["weather"][0]["id"].set(true);
    JsonDocument doc; // NOLINT(misc-const-correctness)
    const bool deserialization =
        deserializeJson(doc, body.data(), DeserializationOption::Filter(filter)) == DeserializationError::Code::Ok;
    if (deserialization && doc["main"]["temp"].is<float>() && doc["weather"][0]["id"].is<uint16_t>())
    {
        // API 2.5
        condition = getCondition(doc["weather"][0]["id"].as<uint16_t>(), codesets);
        temperature = static_cast<int16_t>(roundf(doc["main"]["temp"].as<float>()));
        return;
    }
    if (deserialization && doc["current"]["temp"].is<float>() && doc["current"]["weather"]["id"].is<uint16_t>())
    {
        // API 3.0
        condition = getCondition(doc["current"]["weather"]["id"].as<uint16_t>(), codesets);
        temperature = static_cast<int16_t>(roundf(doc["current"]["temp"].as<float>()));
        return;
    }
    parts.pop_back();
    ESP_LOGD(name, "unsupported format"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    lastMillis = millis() - interval + (1U << 13U);
}

#endif // WEATHER_OPENWEATHER
