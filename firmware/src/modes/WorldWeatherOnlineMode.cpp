#include "config/constants.h"

#if defined(WORLDWEATHERONLINE_KEY) && ((defined(LATITUDE) && defined(LONGITUDE)) || defined(LOCATION))

#include <HTTPClient.h>

#include "extensions/BuildExtension.h"
#include "modes/WorldWeatherOnlineMode.h"
#include "services/ConnectivityService.h"

#if EXTENSION_BUILD
void WorldWeatherOnlineMode::setup()
{
    (*Build->config)[Config::h][__STRING(WORLDWEATHERONLINE_KEY)] = "REDACTED";
#ifdef WORLDWEATHERONLINE_UNIT
    (*Build->config)[Config::h][__STRING(WORLDWEATHERONLINE_UNIT)] = WORLDWEATHERONLINE_UNIT;
#endif
#ifdef WORLDWEATHERONLINE_PARAMETERS
    (*Build->config)[Config::h][__STRING(WORLDWEATHERONLINE_PARAMETERS)] = WORLDWEATHERONLINE_PARAMETERS;
#endif
}
#endif // EXTENSION_BUILD

void WorldWeatherOnlineMode::wake()
{
#ifdef F_INFO
    if (urls.empty())
    {
        Serial.printf("%s: unable to fetch weather\n", name);
    }
    else
    {
        lastMillis = 0;
    }
#else
    lastMillis = 0;
#endif // F_INFO
}

void WorldWeatherOnlineMode::handle()
{
    // World Weather Online update interval: 10-15 minutes
    if (WiFi.isConnected() && urls.size() && (millis() - lastMillis > 1000 * 60 * 10 || lastMillis == 0))
    {
        update();
    }
}

void WorldWeatherOnlineMode::update()
{
    lastMillis = millis();
    HTTPClient http;

    http.begin(urls.back());
    http.addHeader("Accept", "application/json");
    http.setUserAgent(Connectivity.userAgent.data());

#ifdef F_DEBUG
    Serial.printf("%s: %s\n", name, urls.back());
#endif

    const int code = http.GET();
    if (code == t_http_codes::HTTP_CODE_OK)
    {
        JsonDocument doc;
        if (deserializeJson(doc, http.getString()) ||
#ifdef WORLDWEATHERONLINE_UNIT
            !(doc["data"]["current_condition"][0]["temp_" WORLDWEATHERONLINE_UNIT].is<float>() || doc["data"]["current_condition"][0]["temp_" WORLDWEATHERONLINE_UNIT].is<String>()) ||
#else
            !(doc["data"]["current_condition"][0]["temp_C"].is<float>() || doc["data"]["current_condition"][0]["temp_C"].is<String>()) ||
#endif
            !(doc["data"]["current_condition"][0]["weatherCode"].is<uint16_t>() || doc["data"]["current_condition"][0]["weatherCode"].is<String>()))
        {
            urls.pop_back();
            lastMillis = 0;
#ifdef F_DEBUG
            Serial.printf("%s: unprocessable data\n", name);
#endif
            return;
        }
        WeatherHandler weather = WeatherHandler();
#ifdef WORLDWEATHERONLINE_UNIT
        weather.temperature = round(doc["data"]["current_condition"][0]["temp_" WORLDWEATHERONLINE_UNIT].as<float>());
#else
        weather.temperature = round(doc["data"]["current_condition"][0]["temp_C"].as<float>());
#endif
        weather.parse(doc["data"]["current_condition"][0]["weatherCode"].as<uint16_t>(), codesets);
        weather.draw();
    }
    else if (code < 0 || (code >= 400 && code < 500))
    {
        urls.pop_back();
        lastMillis = 0;
#ifdef F_INFO
        if (urls.empty())
        {
            Serial.printf("%s: unable to fetch weather\n", name);
        }
#endif
    }
}

#endif // defined(WORLDWEATHERONLINE_KEY) && ((defined(LATITUDE) && defined(LONGITUDE)) || defined(LOCATION))
