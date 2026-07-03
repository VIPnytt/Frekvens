#if WEATHER_TOMORROWIO

#include "middlewares/TomorrowIoMiddleware.h"

#include <ArduinoJson.h> // NOLINT(misc-include-cleaner)

void TomorrowIoMiddleware::update(std::optional<WeatherHandler::Condition> &condition,
                                  std::optional<int16_t> &temperature, unsigned long &lastMillis)
{
    if (queries.empty())
    {
        ESP_LOGE(name.data(), "weather provider unavailable"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        return;
    }
    query = queries.back();
    std::vector<char> body;
    const int status{fetch(body, lastMillis)};
    if (status != 200)
    {
        if (status >= 400 && status < 500)
        {
            queries.pop_back();
            lastMillis = millis() - interval + (0b1U << 12U);
        }
        return;
    }
    JsonDocument filter; // NOLINT(misc-const-correctness)
    filter["data"]["values"]["temperature"].set(true);
    filter["data"]["values"]["weatherCode"].set(true);
    JsonDocument doc; // NOLINT(misc-const-correctness)
    if (deserializeJson(doc, body.data(), DeserializationOption::Filter(filter)) == DeserializationError::Code::Ok &&
        doc["data"]["values"]["temperature"].is<float>() && doc["data"]["values"]["weatherCode"].is<uint32_t>())
    {
        condition = getCondition<uint32_t>(doc["data"]["values"]["weatherCode"].as<uint32_t>(), codesets);
#if TEMPERATURE_KELVIN
        temperature = static_cast<int16_t>(lroundf(273.15F + doc["data"]["values"]["temperature"].as<float>()));
#else
        temperature = static_cast<int16_t>(lroundf(doc["data"]["values"]["temperature"].as<float>()));
#endif // TEMPERATURE_KELVIN
        return;
    }
    queries.pop_back();
    ESP_LOGD(name.data(), "unsupported format"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    lastMillis = millis() - interval + (0b1U << 13U);
}

#endif // WEATHER_TOMORROWIO
