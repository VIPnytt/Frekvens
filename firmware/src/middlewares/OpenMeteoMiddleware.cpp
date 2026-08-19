#if WEATHER_OPENMETEO

#include "middlewares/OpenMeteoMiddleware.h"

#include <ArduinoJson.h> // NOLINT(misc-include-cleaner)

/**
 * @brief Updates the weather condition and temperature from the configured provider.
 *
 * Removes the current provider and schedules an earlier retry when the provider
 * returns a client error or an unsupported response format.
 *
 * @param condition Weather condition to update.
 * @param temperature Temperature to update, in the configured unit.
 * @param lastMillis Timestamp used to schedule the next update.
 */
void OpenMeteoMiddleware::update(std::optional<WeatherHandler::Condition> &condition,
                                 std::optional<int16_t> &temperature, unsigned long &lastMillis)
{
    if (parts.empty())
    {
        ESP_LOGE(name.data(), "weather provider unavailable"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        return;
    }
    host = parts.back().first;
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
    JsonDocument filter{};
    filter["current"]["temperature_2m"].set(true);
    filter["current"]["weather_code"].set(true);
    JsonDocument doc{};
    if (deserializeJson(doc, body.data(), DeserializationOption::Filter(filter)) == DeserializationError::Code::Ok &&
        doc["current"]["temperature_2m"].is<float>() && doc["current"]["weather_code"].is<uint8_t>())
    {
        condition = getCondition<uint8_t>(doc["current"]["weather_code"].as<uint8_t>(), codesets);
#if TEMPERATURE_KELVIN
        temperature = static_cast<int16_t>(lroundf(273.15F + doc["current"]["temperature_2m"].as<float>()));
#else
        temperature = static_cast<int16_t>(lroundf(doc["current"]["temperature_2m"].as<float>()));
#endif // TEMPERATURE_KELVIN
        return;
    }
    parts.pop_back();
    ESP_LOGD(name.data(), "unsupported format"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    lastMillis = millis() - interval + (0b1U << 13U);
}

#endif // WEATHER_OPENMETEO
