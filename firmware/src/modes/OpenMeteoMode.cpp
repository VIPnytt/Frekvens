#if MODE_OPENMETEO

#include "modes/OpenMeteoMode.h"

#include "services/ConnectivityService.h"

#include <HTTPClient.h>
#include <NetworkClientSecure.h>

void OpenMeteoMode::begin()
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

void OpenMeteoMode::handle()
{
    if (urls.size() && WiFi.isConnected() && millis() - lastMillis >= interval)
    {
        update();
    }
}

void OpenMeteoMode::update()
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
        filter["current"]["temperature_2m"].set(true);
        filter["current"]["weather_code"].set(true);
        JsonDocument doc; // NOLINT(misc-const-correctness)
        if (deserializeJson(doc, stream, DeserializationOption::Filter(filter)) ||
            !doc["current"]["temperature_2m"].is<float>() || !doc["current"]["weather_code"].is<uint8_t>())
        {
            urls.pop_back();
            lastMillis = millis() - interval + (1UL << 14U);
            ESP_LOGD(name, "unprocessable data");
            return;
        }
        WeatherHandler weather = WeatherHandler();
        weather.temperature = round(doc["current"]["temperature_2m"].as<float>());
        weather.parse(doc["current"]["weather_code"].as<uint8_t>(), codesets);
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

#endif // MODE_OPENMETEO
