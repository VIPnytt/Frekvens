#if MODE_YR

#include "modes/YrMode.h"

#include "services/ConnectivityService.h"

#include <HTTPClient.h>
#include <NetworkClientSecure.h>

void YrMode::begin()
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

void YrMode::handle()
{
    if (urls.size() && WiFi.isConnected() && millis() - lastMillis >= interval)
    {
        update();
    }
}

void YrMode::update()
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
        filter["properties"]["timeseries"][0]["data"]["instant"]["details"]["air_temperature"] = true;
        filter["properties"]["timeseries"][0]["data"]["next_1_hours"]["summary"]["symbol_code"] = true;
        JsonDocument doc;
        if (deserializeJson(doc, stream, DeserializationOption::Filter(filter)) || !doc["properties"]["timeseries"][0]["data"]["instant"]["details"]["air_temperature"].is<float>() || !doc["properties"]["timeseries"][0]["data"]["next_1_hours"]["summary"]["symbol_code"].is<std::string>())
        {
            urls.pop_back();
            lastMillis = millis() - interval + (1 << 14);
            ESP_LOGD(name, "unprocessable data");
            return;
        }
        WeatherHandler weather = WeatherHandler();
        weather.temperature = round(doc["properties"]["timeseries"][0]["data"]["instant"]["details"]["air_temperature"].as<float>());
        weather.parse(doc["properties"]["timeseries"][0]["data"]["next_1_hours"]["summary"]["symbol_code"].as<std::string>(), codesets);
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

#endif // MODE_YR
