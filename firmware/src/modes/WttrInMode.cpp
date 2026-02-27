#if MODE_WTTRIN

#include "modes/WttrInMode.h"

#include "services/ConnectivityService.h"

#include <HTTPClient.h>
#include <NetworkClientSecure.h>

void WttrInMode::begin()
{
    if (urls.empty())
    {
        ESP_LOGW(name, "unable to fetch weather");
    }
    else
    {
        lastMillis = millis() - interval;
    }
}

void WttrInMode::handle()
{
    if (urls.size() && WiFi.isConnected() && millis() - lastMillis >= interval)
    {
        update();
    }
}

void WttrInMode::update()
{
    lastMillis = millis();
    NetworkClientSecure client; // NOLINT(misc-const-correctness)
    const std::span<const uint8_t> bundle = ConnectivityService::certificates();
    client.setCACertBundle(bundle.data(), bundle.size());
    HTTPClient http;
    http.begin(client, urls.back());
    http.addHeader("Accept", "application/json");
    http.setUserAgent(Connectivity.userAgent.data());
    const int code = http.GET();
    if (code == t_http_codes::HTTP_CODE_OK)
    {
        NetworkClient &stream = http.getStream();
        const int contentLength = http.getSize();
        const unsigned long _lastMillis = millis();
        while (stream.available() < contentLength && millis() - _lastMillis < (1UL << 13U))
        {
            vTaskDelay(1);
        }
        JsonDocument filter; // NOLINT(misc-const-correctness)
#if TEMPERATURE_FAHRENHEIT
        filter["current_condition"][0]["temp_F"].set(true);
#else
        filter["current_condition"][0]["temp_C"].set(true);
#endif // TEMPERATURE_FAHRENHEIT
        filter["current_condition"][0]["weatherCode"].set(true);
        JsonDocument doc; // NOLINT(misc-const-correctness)
        if (deserializeJson(doc, stream, DeserializationOption::Filter(filter)) ||
#if TEMPERATURE_FAHRENHEIT
            !(doc["current_condition"][0]["temp_F"].is<float>() ||
              doc["current_condition"][0]["temp_F"].is<std::string>()) ||
#else
            !(doc["current_condition"][0]["temp_C"].is<float>() ||
              doc["current_condition"][0]["temp_C"].is<std::string>()) ||
#endif // TEMPERATURE_FAHRENHEIT
            !(doc["current_condition"][0]["weatherCode"].is<uint16_t>() ||
              doc["current_condition"][0]["weatherCode"].is<std::string>()))
        {
            urls.pop_back();
            lastMillis = millis() - interval + (1UL << 14U);
            ESP_LOGD(name, "unprocessable data");
            return;
        }
        WeatherHandler weather;
#if TEMPERATURE_FAHRENHEIT
        weather.temperature = round(doc["current_condition"][0]["temp_F"].as<float>());
#else
        weather.temperature = round(doc["current_condition"][0]["temp_C"].as<float>());
#endif // TEMPERATURE_FAHRENHEIT
        weather.parse(doc["current_condition"][0]["weatherCode"].as<uint16_t>(), codesets);
        weather.draw();
    }
    else if (code >= 400 && code < 500)
    {
        urls.pop_back();
        lastMillis = millis() - interval + (1UL << 12U);
        if (urls.empty())
        {
            ESP_LOGE(name, "unable to fetch weather");
        }
    }
    else if (code < 0)
    {
        lastMillis = millis() - interval + (1UL << 15U);
    }
}

#endif // MODE_WTTRIN
