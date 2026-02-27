#if MODE_GOOGLEWEATHER

#include "modes/GoogleWeatherMode.h"

#include "services/ConnectivityService.h"

#include <HTTPClient.h>
#include <NetworkClientSecure.h>

void GoogleWeatherMode::begin()
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

void GoogleWeatherMode::handle()
{
    if (urls.size() && WiFi.isConnected() && millis() - lastMillis >= interval)
    {
        update();
    }
}

void GoogleWeatherMode::update()
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
        JsonDocument filter; // NOLINT(misc-const-correctness)
        filter["temperature"]["degrees"].set(true);
        filter["weatherCondition"]["type"].set(true);
        JsonDocument doc; // NOLINT(misc-const-correctness)
        if (deserializeJson(doc, http.getString(), DeserializationOption::Filter(filter)) ||
            !doc["temperature"]["degrees"].is<float>() || !doc["weatherCondition"]["type"].is<std::string>())
        {
            urls.pop_back();
            lastMillis = millis() - interval + (1UL << 14U);
            ESP_LOGD(name, "unprocessable data");
            return;
        }
        WeatherHandler weather = WeatherHandler();
        weather.temperature = round(doc["temperature"]["degrees"].as<float>());
        weather.parse(doc["weatherCondition"]["type"].as<std::string>(), codesets);
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

#endif // MODE_GOOGLEWEATHER
