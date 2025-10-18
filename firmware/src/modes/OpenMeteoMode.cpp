#include "config/constants.h"

#if MODE_OPENMETEO && defined(LATITUDE) && defined(LONGITUDE)

#include <HTTPClient.h>

#include "extensions/BuildExtension.h"
#include "modes/OpenMeteoMode.h"
#include "services/ConnectivityService.h"

#if EXTENSION_BUILD && (defined(OPENMETEO_KEY) || defined(OPENMETEO_PARAMETERS))
void OpenMeteoMode::setup()
{
#ifdef OPENMETEO_KEY
    (*Build->config)[Config::h][__STRING(OPENMETEO_KEY)] = "REDACTED";
#endif // OPENMETEO_KEY
#ifdef OPENMETEO_PARAMETERS
    (*Build->config)[Config::h][__STRING(OPENMETEO_PARAMETERS)] = OPENMETEO_PARAMETERS;
#endif // OPENMETEO_PARAMETERS
}
#endif // EXTENSION_BUILD && (defined(OPENMETEO_KEY) || defined(OPENMETEO_PARAMETERS))

void OpenMeteoMode::wake()
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

void OpenMeteoMode::handle()
{
    // Open-Meteo data resolution: down to 15 minutes (depending on location)
    if (WiFi.isConnected() && urls.size() && (millis() - lastMillis > 900'000 || lastMillis == 0))
    {
        update();
    }
}

void OpenMeteoMode::update()
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
        if (deserializeJson(doc, http.getString()) || !doc["current"]["temperature_2m"].is<float>() || !doc["current"]["weather_code"].is<uint8_t>())
        {
            urls.pop_back();
            lastMillis = 0;
#ifdef F_DEBUG
            Serial.printf("%s: unprocessable data\n", name);
#endif
            return;
        }
        WeatherHandler weather = WeatherHandler();
        weather.temperature = round(doc["current"]["temperature_2m"].as<float>());
        weather.parse(doc["current"]["weather_code"].as<uint8_t>(), codesets);
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

#endif // MODE_OPENMETEO && defined(LATITUDE) && defined(LONGITUDE)
