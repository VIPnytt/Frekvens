#include "config/constants.h"

#if defined(OPENWEATHER_KEY) && defined(LATITUDE) && defined(LONGITUDE)

#include <HTTPClient.h>

#include "extensions/BuildExtension.h"
#include "modes/OpenWeatherMode.h"
#include "services/NetworkService.h"

#if EXTENSION_BUILD
void OpenWeatherMode::setup()
{
    (*Build->config)[Config::h][__STRING(OPENWEATHER_KEY)] = "REDACTED";
#ifdef OPENWEATHER_PARAMETERS
    (*Build->config)[Config::h][__STRING(OPENWEATHER_PARAMETERS)] = OPENWEATHER_PARAMETERS;
#endif
}
#endif // EXTENSION_BUILD

void OpenWeatherMode::wake()
{
    lastMillis = 0;
}

void OpenWeatherMode::handle()
{
    // OpenWeather's recommended update interval: 10 minutes
    if (WiFi.isConnected() && urls.size() && (millis() - lastMillis > 1000 * 60 * 10 || lastMillis == 0))
    {
        update();
    }
}

void OpenWeatherMode::update()
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
        const bool isError = (bool)deserializeJson(doc, http.getString());
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
            lastMillis = 0;
#ifdef F_DEBUG
            Serial.print(name);
            Serial.println(": unprocessable data");
#endif
        }
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

#endif // defined(LATITUDE) && defined(LONGITUDE) && defined(OPENWEATHER_KEY)
