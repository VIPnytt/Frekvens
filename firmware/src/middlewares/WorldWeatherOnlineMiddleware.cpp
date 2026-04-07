#if WEATHER_WORLDWEATHERONLINE

#include "middlewares/WorldWeatherOnlineMiddleware.h"

#include <ArduinoJson.h>

void WorldWeatherOnlineMiddleware::update(std::optional<WeatherHandler::Conditions> &condition,
                                          std::optional<int16_t> &temperature, unsigned long &lastMillis)
{
    if (queries.empty())
    {
        ESP_LOGE(name, "weather provider unavailable"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        return;
    }
    query = queries.back();
    std::vector<char> body;
    const int status = fetch(body, lastMillis);
    if (status != 200)
    {
        if (status >= 400 && status < 500)
        {
            queries.pop_back();
            lastMillis = millis() - interval + (1U << 12U);
        }
        return;
    }
    JsonDocument filter; // NOLINT(misc-const-correctness)
#if TEMPERATURE_FAHRENHEIT
    filter["data"]["current_condition"][0]["temp_F"].set(true);
#else
    filter["data"]["current_condition"][0]["temp_C"].set(true);
#endif // TEMPERATURE_FAHRENHEIT
    filter["data"]["current_condition"][0]["weatherCode"].set(true);
    JsonDocument doc; // NOLINT(misc-const-correctness)
    if (deserializeJson(doc, body.data(), DeserializationOption::Filter(filter)) == DeserializationError::Code::Ok &&
#if TEMPERATURE_FAHRENHEIT
        doc["data"]["current_condition"][0]["temp_F"].is<std::string_view>() &&
#else
        doc["data"]["current_condition"][0]["temp_C"].is<std::string_view>() &&
#endif // TEMPERATURE_FAHRENHEIT
        doc["data"]["current_condition"][0]["weatherCode"].is<std::string_view>())
    {
        condition = getCondition(doc["data"]["current_condition"][0]["weatherCode"].as<uint16_t>(), codesets);
#if TEMPERATURE_FAHRENHEIT
        temperature = doc["data"]["current_condition"][0]["temp_F"].as<int16_t>();
#else
        temperature = doc["data"]["current_condition"][0]["temp_C"].as<int16_t>();
#endif // TEMPERATURE_FAHRENHEIT
        return;
    }
    queries.pop_back();
    ESP_LOGD(name, "unsupported format"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    lastMillis = millis() - interval + (1U << 13U);
}

#endif // WEATHER_WORLDWEATHERONLINE
