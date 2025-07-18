#include "config/constants.h"

#if MODE_HOMEASSISTANTWEATHER

#include <HTTPClient.h>

#include "extensions/BuildExtension.h"
#include "modes/HomeAssistantWeatherMode.h"
#include "services/NetworkService.h"

#if EXTENSION_BUILD
void HomeAssistantWeatherMode::setup()
{
#ifdef HOMEASSISTANT_ENTITY
    (*Build->config)[Config::h][__STRING(HOMEASSISTANT_ENTITY)] = HOMEASSISTANT_ENTITY;
#endif // HOMEASSISTANT_ENTITY
    (*Build->config)[Config::h][__STRING(HOMEASSISTANT_KEY)] = "REDACTED";
#ifdef HOMEASSISTANT_HOST
    (*Build->config)[Config::h][__STRING(HOMEASSISTANT_HOST)] = HOMEASSISTANT_HOST;
#endif // HOMEASSISTANT_HOST
#ifdef HOMEASSISTANT_PORT
    (*Build->config)[Config::h][__STRING(HOMEASSISTANT_PORT)] = HOMEASSISTANT_PORT;
#endif // HOMEASSISTANT_PORT
}
#endif // EXTENSION_BUILD

void HomeAssistantWeatherMode::wake()
{
    lastMillis = 0;
}

void HomeAssistantWeatherMode::handle()
{
    if (WiFi.isConnected() && urls.size() && (millis() - lastMillis > UINT16_MAX || lastMillis == 0))
    {
        update();
    }
}

void HomeAssistantWeatherMode::update()
{
    lastMillis = millis();
    HTTPClient http;

    http.begin(urls.back().c_str());
    http.addHeader("Accept", "application/json");
    http.setAuthorizationType("Bearer");
    http.setAuthorization(HOMEASSISTANT_KEY);
    http.setUserAgent(Network.userAgent.data());

#ifdef F_DEBUG
    Serial.print(name);
    Serial.print(": GET ");
    Serial.println(urls.back().c_str());
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
        if (deserializeJson(doc, http.getString()) || !doc["attributes"]["temperature"].is<float>() || !doc["state"].is<std::string>())
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
        weather.temperature = round(doc["attributes"]["temperature"].as<float>());
        weather.parse(doc["state"].as<std::string>(), codesets);
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

#endif // MODE_HOMEASSISTANTWEATHER
