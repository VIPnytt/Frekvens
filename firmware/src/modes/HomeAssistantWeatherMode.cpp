#if MODE_HOMEASSISTANTWEATHER

#include "modes/HomeAssistantWeatherMode.h"

#include "services/ConnectivityService.h"

#include <HTTPClient.h>
#include <NetworkClientSecure.h>

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
    NetworkClientSecure client;
    client.setCACertBundle(Certificates::x509_crt_bundle_start, Certificates::x509_crt_bundle_end - Certificates::x509_crt_bundle_start);
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
        while (stream.available() < contentLength && millis() - _lastMillis < (1 << 13))
        {
            vTaskDelay(1);
        }
        JsonDocument filter;
        filter["attributes"]["temperature"] = true;
        filter["state"] = true;
        JsonDocument doc;
        if (deserializeJson(doc, stream, DeserializationOption::Filter(filter)) || !doc["attributes"]["temperature"].is<float>() || !doc["state"].is<std::string>())
        {
            urls.pop_back();
            lastMillis = millis() - interval + (1 << 14);
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

#endif // MODE_HOMEASSISTANTWEATHER
