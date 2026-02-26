#if MODE_HOMEASSISTANTWEATHER

#include "modes/HomeAssistantWeatherMode.h"

#include "services/ConnectivityService.h"

#include <HTTPClient.h>
#include <NetworkClientSecure.h> // NOLINT(misc-include-cleaner)

void HomeAssistantWeatherMode::begin()
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

void HomeAssistantWeatherMode::handle()
{
    if (urls.size() && WiFi.isConnected() && millis() - lastMillis >= interval)
    {
        update();
    }
}

void HomeAssistantWeatherMode::update()
{
    lastMillis = millis();
#ifdef HOMEASSISTANT_PROTOCOL
    NetworkClientSecure client; // NOLINT(misc-const-correctness)
    const std::span<const uint8_t> bundle = ConnectivityService::certificates();
    client.setCACertBundle(bundle.data(), bundle.size());
    HTTPClient http;
    http.begin(client, urls.back().c_str());
#else
    HTTPClient http;
    http.begin(urls.back().c_str());
#endif // HOMEASSISTANT_PROTOCOL
    http.addHeader("Accept", "application/json");
    http.setAuthorizationType("Bearer");
    http.setAuthorization(HOMEASSISTANT_KEY);
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
        filter["attributes"]["temperature"].set(true);
        filter["state"].set(true);
        JsonDocument doc; // NOLINT(misc-const-correctness)
        if (deserializeJson(doc, stream, DeserializationOption::Filter(filter)) ||
            !doc["attributes"]["temperature"].is<float>() || !doc["state"].is<std::string>())
        {
            urls.pop_back();
            lastMillis = millis() - interval + (1UL << 14U);
            ESP_LOGD(name, "unprocessable data");
            return;
        }
        WeatherHandler weather = WeatherHandler();
        weather.temperature = round(doc["attributes"]["temperature"].as<float>());
        weather.parse(doc["state"].as<std::string>(), codesets);
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

#endif // MODE_HOMEASSISTANTWEATHER
