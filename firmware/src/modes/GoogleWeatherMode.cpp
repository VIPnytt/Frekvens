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
    NetworkClientSecure client;
    client.setCACertBundle(Certificates::x509_crt_bundle_start,
                           Certificates::x509_crt_bundle_end - Certificates::x509_crt_bundle_start);
    HTTPClient http;
    http.begin(client, urls.back());
    http.addHeader("Accept", "application/json");
    http.setUserAgent(Connectivity.userAgent.data());
    const int code = http.GET();
    if (code == t_http_codes::HTTP_CODE_OK)
    {
        JsonDocument filter;
        filter["temperature"]["degrees"] = true;
        filter["weatherCondition"]["type"] = true;
        JsonDocument doc;
        if (deserializeJson(doc, http.getString(), DeserializationOption::Filter(filter)) ||
            !doc["temperature"]["degrees"].is<float>() || !doc["weatherCondition"]["type"].is<std::string>())
        {
            urls.pop_back();
            lastMillis = millis() - interval + (1 << 14);
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
        lastMillis = millis() - interval + (1 << 12);
        if (urls.empty())
        {
            ESP_LOGE(name, "unable to fetch weather");
        }
    }
    else if (code < 0)
    {
        lastMillis = millis() - interval + (1 << 15);
    }
}

#endif // MODE_GOOGLEWEATHER
