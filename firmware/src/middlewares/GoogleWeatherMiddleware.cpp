#if WEATHER_GOOGLEWEATHER

#include "middlewares/GoogleWeatherMiddleware.h"

#include <ArduinoJson.h> // NOLINT(misc-include-cleaner)

void GoogleWeatherMiddleware::update(std::optional<WeatherHandler::Conditions> &condition,
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
    filter["temperature"]["degrees"].set(true);
    filter["weatherCondition"]["type"].set(true);
    JsonDocument doc; // NOLINT(misc-const-correctness)
    if (deserializeJson(doc, body.data(), DeserializationOption::Filter(filter)) == DeserializationError::Code::Ok &&
        doc["temperature"]["degrees"].is<float>() && doc["weatherCondition"]["type"].is<std::string_view>())
    {
        condition = getCondition(doc["weatherCondition"]["type"].as<std::string_view>(), codesets);
        temperature = static_cast<int16_t>(roundf(doc["temperature"]["degrees"].as<float>()));
        return;
    }
    queries.pop_back();
    ESP_LOGD(name, "unsupported format"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    lastMillis = millis() - interval + (1U << 13U);
}

#endif // WEATHER_GOOGLEWEATHER
