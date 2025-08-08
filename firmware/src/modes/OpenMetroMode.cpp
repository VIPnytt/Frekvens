#include "config/constants.h"

#if defined(LATITUDE) && defined(LONGITUDE)

#include <HTTPClient.h>

#include "extensions/BuildExtension.h"
#include "modes/OpenMetroMode.h"
#include "services/NetworkService.h"

#if EXTENSION_BUILD && (defined(OPENMETRO_KEY) || defined(OPENMETRO_PARAMETERS))
void OpenMetroMode::setup()
{
#ifdef OPENMETRO_KEY
    (*Build->config)[Config::h][__STRING(OPENMETRO_KEY)] = "REDACTED";
#endif // OPENMETRO_KEY
#ifdef OPENMETRO_PARAMETERS
    (*Build->config)[Config::h][__STRING(OPENMETRO_PARAMETERS)] = OPENMETRO_PARAMETERS;
#endif // OPENMETRO_PARAMETERS
}
#endif // EXTENSION_BUILD && (defined(OPENMETRO_KEY) || defined(OPENMETRO_PARAMETERS))

void OpenMetroMode::wake()
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

void OpenMetroMode::handle()
{
    // Open-Metro data resolution: down to 15 minutes (depending on location)
    if (WiFi.isConnected() && urls.size() && (millis() - lastMillis > 1000 * 60 * 15 || lastMillis == 0))
    {
        update();
    }
}

void OpenMetroMode::update()
{
    lastMillis = millis();
    HTTPClient http;

    http.begin(urls.back());
    http.addHeader("Accept", "application/json");
    http.setUserAgent(Network.userAgent.data());

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

#endif // defined(LATITUDE) && defined(LONGITUDE)
