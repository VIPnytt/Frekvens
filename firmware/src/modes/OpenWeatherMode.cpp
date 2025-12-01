#if MODE_OPENWEATHER

#include <HTTPClient.h>
#include <NetworkClientSecure.h>

#include "modes/OpenWeatherMode.h"
#include "services/ConnectivityService.h"

void OpenWeatherMode::begin()
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

void OpenWeatherMode::handle()
{
    if (urls.size() && WiFi.isConnected() && millis() - lastMillis >= interval)
    {
        update();
    }
}

void OpenWeatherMode::update()
{
    lastMillis = millis();
    NetworkClientSecure client;
    client.setCACertBundle(Certificates::x509_crt_bundle_start, Certificates::x509_crt_bundle_end - Certificates::x509_crt_bundle_start);
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
        while (stream.available() < contentLength && millis() - _lastMillis < (1 << 13))
        {
            vTaskDelay(1);
        }
        JsonDocument filter;
        filter["current"]["temp"] = true;
        filter["current"]["weather"]["id"] = true;
        filter["main"]["temp"] = true;
        filter["weather"][0]["id"] = true;
        JsonDocument doc;
        const bool isError = (bool)deserializeJson(doc, stream, DeserializationOption::Filter(filter));
        if (!isError && doc["current"]["temp"].is<float>() && doc["current"]["weather"]["id"].is<uint16_t>())
        {
            // API 3.0
            WeatherHandler weather = WeatherHandler();
            weather.temperature = round(doc["current"]["temp"].as<float>());
            weather.parse(doc["current"]["weather"]["id"].as<uint16_t>(), codesets);
            weather.draw();
        }
        else if (!isError && doc["main"]["temp"].is<float>() && doc["weather"][0]["id"].is<uint16_t>())
        {
            // API 2.5
            WeatherHandler weather = WeatherHandler();
            weather.temperature = round(doc["main"]["temp"].as<float>());
            weather.parse(doc["weather"][0]["id"].as<uint16_t>(), codesets);
            weather.draw();
        }
        else
        {
            urls.pop_back();
            lastMillis = millis() - interval + (1 << 14);
            ESP_LOGD(name, "unprocessable data");
        }
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

#endif // MODE_OPENWEATHER
