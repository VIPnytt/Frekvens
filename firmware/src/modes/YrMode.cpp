#include "config/constants.h"

#if defined(LATITUDE) && defined(LONGITUDE)

#include <HTTPClient.h>

#include "extensions/BuildExtension.h"
#include "modes/YrMode.h"
#include "services/NetworkService.h"

#if EXTENSION_BUILD && defined(YR_PARAMETERS)
void YrMode::setup()
{
    (*Build->config)[Config::h][__STRING(YR_PARAMETERS)] = YR_PARAMETERS;
}
#endif // EXTENSION_BUILD && defined(YR_PARAMETERS)

void YrMode::wake()
{
#ifdef F_INFO
    if (urls.empty())
    {
        Serial.print(name);
        Serial.println(": unable to fetch weather");
    }
    else
    {
        lastMillis = 0;
    }
#else
    lastMillis = 0;
#endif // F_INFO
}

void YrMode::handle()
{
    // Yr data resolution: down to 5 minutes (depending on location)
    if (WiFi.isConnected() && urls.size() && (millis() - lastMillis > 1000 * 60 * 5 || lastMillis == 0))
    {
        update();
    }
}

void YrMode::update()
{
    lastMillis = millis();
    HTTPClient http;

    http.begin(urls.back());
    http.addHeader("Accept", "application/json");
    http.setUserAgent(Network.userAgent.data());

#ifdef F_DEBUG
    Serial.print(name);
    Serial.print(": ");
    Serial.println(urls.back());
#endif

    const int code = http.GET();
    if (code == t_http_codes::HTTP_CODE_OK)
    {
        JsonDocument doc;
        if (deserializeJson(doc, http.getString()) || !doc["properties"]["timeseries"][0]["data"]["instant"]["details"]["air_temperature"].is<float>() || !doc["properties"]["timeseries"][0]["data"]["next_1_hours"]["summary"]["symbol_code"].is<std::string>())
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
        weather.temperature = round(doc["properties"]["timeseries"][0]["data"]["instant"]["details"]["air_temperature"].as<float>());
        weather.parse(doc["properties"]["timeseries"][0]["data"]["next_1_hours"]["summary"]["symbol_code"].as<std::string>(), codesets);
        weather.draw();
    }
    else if (code < 0 || (code >= 400 && code < 500))
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

#endif // defined(LATITUDE) && defined(LONGITUDE)
