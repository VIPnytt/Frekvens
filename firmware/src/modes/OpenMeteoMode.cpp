#include "config/constants.h"

#if MODE_OPENMETEO

#include <HTTPClient.h>
#include <NetworkClientSecure.h>

#include "modes/OpenMeteoMode.h"
#include "services/ConnectivityService.h"

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
        filter["current"]["temperature_2m"] = true;
        filter["current"]["weather_code"] = true;
        JsonDocument doc;
        if (deserializeJson(doc, stream, DeserializationOption::Filter(filter)) || !doc["current"]["temperature_2m"].is<float>() || !doc["current"]["weather_code"].is<uint8_t>())
        {
            urls.pop_back();
            lastMillis = millis() - interval + (1 << 14);
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

#endif // MODE_OPENMETEO
