#include "config/constants.h"

#if MODE_WORLDWEATHERONLINE

#include <HTTPClient.h>
#include <NetworkClientSecure.h>

#include "modes/WorldWeatherOnlineMode.h"
#include "services/ConnectivityService.h"

void WorldWeatherOnlineMode::wake()
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

void WorldWeatherOnlineMode::handle()
{
    if (urls.size() && WiFi.isConnected() && millis() - lastMillis >= interval)
    {
        update();
    }
}

void WorldWeatherOnlineMode::update()
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
        filter["data"]["current_condition"][0]["temp_F"] = true;
#else
        filter["data"]["current_condition"][0]["temp_C"] = true;
#endif
        filter["data"]["current_condition"][0]["weatherCode"] = true;
        JsonDocument doc;
        if (deserializeJson(doc, stream, DeserializationOption::Filter(filter)) ||
#if TEMPERATURE_FAHRENHEIT
            !(doc["data"]["current_condition"][0]["temp_F"].is<float>() || doc["data"]["current_condition"][0]["temp_F"].is<std::string>()) ||
#else
            !(doc["data"]["current_condition"][0]["temp_C"].is<float>() || doc["data"]["current_condition"][0]["temp_C"].is<std::string>()) ||
#endif
            !(doc["data"]["current_condition"][0]["weatherCode"].is<uint16_t>() || doc["data"]["current_condition"][0]["weatherCode"].is<std::string>()))
        {
            urls.pop_back();
            lastMillis = millis() - interval + (1 << 14);
            ESP_LOGD(name, "unprocessable data");
            return;
        }
        WeatherHandler weather = WeatherHandler();
#if TEMPERATURE_FAHRENHEIT
        weather.temperature = round(doc["data"]["current_condition"][0]["temp_F"].as<float>());
#else
        weather.temperature = round(doc["data"]["current_condition"][0]["temp_C"].as<float>());
#endif
        weather.parse(doc["data"]["current_condition"][0]["weatherCode"].as<uint16_t>(), codesets);
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

#endif // MODE_WORLDWEATHERONLINE
