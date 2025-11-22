#include "config/constants.h"

#if MODE_WTTRIN

#include <HTTPClient.h>
#include <NetworkClientSecure.h>

#include "modes/WttrInMode.h"
#include "services/ConnectivityService.h"

void WttrInMode::wake()
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
#if TEMPERATURE_FAHRENHEIT
        filter["current_condition"][0]["temp_F"] = true;
#else
        filter["current_condition"][0]["temp_C"] = true;
#endif // TEMPERATURE_FAHRENHEIT
        filter["current_condition"][0]["weatherCode"] = true;
        JsonDocument doc;
        if (deserializeJson(doc, stream, DeserializationOption::Filter(filter)) ||
#if TEMPERATURE_FAHRENHEIT
            !(doc["current_condition"][0]["temp_F"].is<float>() || doc["current_condition"][0]["temp_F"].is<std::string>()) ||
#else
            !(doc["current_condition"][0]["temp_C"].is<float>() || doc["current_condition"][0]["temp_C"].is<std::string>()) ||
#endif // TEMPERATURE_FAHRENHEIT
            !(doc["current_condition"][0]["weatherCode"].is<uint16_t>() || doc["current_condition"][0]["weatherCode"].is<std::string>()))
        {
            urls.pop_back();
            lastMillis = millis() - interval + (1 << 14);
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

#endif // MODE_WTTRIN
