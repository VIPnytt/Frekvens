#include <HTTPClient.h>

#include "extensions/BuildExtension.h"
#include "modes/WttrInMode.h"
#include "services/NetworkService.h"

#if EXTENSION_BUILD && defined(WTTRIN_PARAMETERS)
void WttrInMode::setup()
{
    (*Build->config)[Config::h][__STRING(WTTRIN_PARAMETERS)] = WTTRIN_PARAMETERS;
}
#endif

void WttrInMode::wake()
{
    lastMillis = 0;
}

void WttrInMode::handle()
{
    // Wttr.in recommended update interval: 1 hour
    if (WiFi.isConnected() && urls.size() && (millis() - lastMillis > 1000 * 60 * 60 * 1 || lastMillis == 0))
    {
        update();
    }
}

void WttrInMode::update()
{
    lastMillis = millis();
    HTTPClient http;

    http.begin(urls.back());
    http.addHeader("Accept", "application/json");
    http.setUserAgent(Network.userAgent.data());

#ifdef F_DEBUG
    Serial.print(name);
    Serial.print(": GET ");
    Serial.println(urls.back());
#endif

    const int code = http.GET();

#ifdef F_VERBOSE
    Serial.print(name);
    Serial.print(": HTTP ");
    Serial.println(code);
#endif

    if (code == t_http_codes::HTTP_CODE_OK)
    {
        JsonDocument doc;
        if (deserializeJson(doc, http.getString()) || !(doc["current_condition"][0]["temp_C"].is<float>() || doc["current_condition"][0]["temp_C"].is<String>()) || !(doc["current_condition"][0]["weatherCode"].is<uint16_t>() || doc["current_condition"][0]["weatherCode"].is<String>()))
        {
            urls.pop_back();
            lastMillis = 0;
#ifdef F_DEBUG
            Serial.print(name);
            Serial.println(": unprocessable data");
#endif
            return;
        }
        WeatherHandler weather = WeatherHandler();
        weather.temperature = round(doc["current_condition"][0]["temp_C"].as<float>());
        weather.parse(doc["current_condition"][0]["weatherCode"].as<uint16_t>(), codesets);
        weather.draw();
    }
    else if (WiFi.isConnected() && (code < 0 || (code >= 400 && code < 500)))
    {
        urls.pop_back();
        lastMillis = 0;
#ifdef F_INFO
        if (urls.empty())
        {
            Serial.print(name);
            Serial.println(": unable to fetch weather");
        }
#endif
    }
}
