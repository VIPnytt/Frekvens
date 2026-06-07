#if WEATHER_OPENWEATHER

#include "middlewares/OpenWeatherMiddleware.h"

#include <ArduinoJson.h> // NOLINT(misc-include-cleaner)

void OpenWeatherMiddleware::update(std::optional<WeatherHandler::Conditions> &condition,
                                   std::optional<int16_t> &temperature, unsigned long &lastMillis)
{
    if (parts.empty())
    {
        ESP_LOGE("Weather", "provider unavailable"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        return;
    }
    path = parts.back().first;
    query = parts.back().second;
    std::vector<char> body;
    const int status{fetch(body, lastMillis)};
    if (status != 200)
    {
        if (status >= 400 && status < 500)
        {
            parts.pop_back();
            lastMillis = millis() - interval + (0b1U << 12U);
        }
        return;
    }
    JsonDocument filter; // NOLINT(misc-const-correctness)
    filter["current"]["temp"].set(true);               // API 3.0
    filter["current"]["weather"][0U]["id"].set(true);  // API 3.0
    filter["data"][0U]["temp"].set(true);              // API 4.0
    filter["data"][0U]["weather"][0U]["id"].set(true); // API 4.0
    filter["main"]["temp"].set(true);                  // API 2.5
    filter["weather"][0U]["id"].set(true);             // API 2.5
    JsonDocument doc; // NOLINT(misc-const-correctness)
    const bool deserialization{deserializeJson(doc, body.data(), DeserializationOption::Filter(filter)) ==
                               DeserializationError::Code::Ok};
    if (deserialization && doc["main"]["temp"].is<float>() && doc["weather"][0U]["id"].is<uint16_t>())
    {
        // API 2.5
        condition = getCondition(doc["weather"][0U]["id"].as<uint16_t>(), codesets);
        temperature = static_cast<int16_t>(lroundf(doc["main"]["temp"].as<float>()));
        return;
    }
    if (deserialization && doc["data"][0U]["temp"].is<float>() && doc["data"][0U]["weather"][0U]["id"].is<uint16_t>())
    {
        // API 4.0
        condition = getCondition(doc["data"][0U]["weather"][0U]["id"].as<uint16_t>(), codesets);
        temperature = static_cast<int16_t>(lroundf(doc["data"][0U]["temp"].as<float>()));
        return;
    }
    if (deserialization && doc["current"]["temp"].is<float>() && doc["current"]["weather"][0U]["id"].is<uint16_t>())
    {
        // API 3.0
        condition = getCondition(doc["current"]["weather"][0U]["id"].as<uint16_t>(), codesets);
        temperature = static_cast<int16_t>(lroundf(doc["current"]["temp"].as<float>()));
        return;
    }
    parts.pop_back();
    ESP_LOGD("Response", "unsupported format"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    lastMillis = millis() - interval + (0b1U << 13U);
}

#endif // WEATHER_OPENWEATHER
