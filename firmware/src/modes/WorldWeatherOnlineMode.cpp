#if MODE_WORLDWEATHERONLINE

#include "modes/WorldWeatherOnlineMode.h"

#include "services/ConnectivityService.h"

#include <esp_crt_bundle.h>
#include <esp_http_client.h>

void WorldWeatherOnlineMode::begin() { lastMillis = millis() - interval; }

void WorldWeatherOnlineMode::handle()
{
    if (WiFi.isConnected() && millis() - lastMillis >= interval)
    {
        update();
    }
}

void WorldWeatherOnlineMode::update()
{
    lastMillis = millis();
    if (queries.empty())
    {
        ESP_LOGE(name, "unable to fetch weather");
        return;
    }
    esp_http_client_config_t config = {
        .host = "api.worldweatheronline.com",
        .port = 443,
        .path = "/premium/v1/weather.ashx",
        .query = queries.back(),
        .user_agent = Connectivity.userAgent.data(),
        .method = esp_http_client_method_t::HTTP_METHOD_GET,
        .transport_type = esp_http_client_transport_t::HTTP_TRANSPORT_OVER_SSL,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == nullptr)
    {
        lastMillis = millis() - interval + (1UL << 15U);
        return;
    }
    esp_http_client_set_header(client, "Accept", "application/json");
    if (esp_http_client_open(client, 0) != ESP_OK)
    {
        esp_http_client_cleanup(client);
        lastMillis = millis() - interval + (1UL << 14U);
        return;
    }
    const int headerLength = esp_http_client_fetch_headers(client);
    if (headerLength < 0)
    {
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        lastMillis = millis() - interval + (1UL << 13U);
        return;
    }
    const int status = esp_http_client_get_status_code(client);
    if (status != 200)
    {
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        if (status >= 400 && status < 500)
        {
            queries.pop_back();
            lastMillis = millis() - interval + (1UL << 12U);
            ESP_LOGV(name, "http status %d", status);
        }
        return;
    }
    const int64_t contentLength = esp_http_client_get_content_length(client);
    std::vector<char> body;
    if (contentLength > 0)
    {
        body.reserve(static_cast<size_t>(contentLength) + 1U);
    }
    std::array<char, 1 << 9> buffer{};
    while (true)
    {
        const int read = esp_http_client_read(client, buffer.data(), static_cast<int>(buffer.size()));
        if (read <= 0)
        {
            break;
        }
        body.insert(body.end(), buffer.data(), buffer.data() + read);
    }
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
    body.push_back('\0');
    JsonDocument filter; // NOLINT(misc-const-correctness)
#if TEMPERATURE_FAHRENHEIT
    filter["data"]["current_condition"][0]["temp_F"].set(true);
#else
    filter["data"]["current_condition"][0]["temp_C"].set(true);
#endif // TEMPERATURE_FAHRENHEIT
    filter["data"]["current_condition"][0]["weatherCode"].set(true);
    JsonDocument doc; // NOLINT(misc-const-correctness)
    if (deserializeJson(doc, body.data(), DeserializationOption::Filter(filter)) != DeserializationError::Code::Ok ||
#if TEMPERATURE_FAHRENHEIT
        !(doc["data"]["current_condition"][0]["temp_F"].is<float>() ||
          doc["data"]["current_condition"][0]["temp_F"].is<std::string>()) ||
#else
        !(doc["data"]["current_condition"][0]["temp_C"].is<float>() ||
          doc["data"]["current_condition"][0]["temp_C"].is<std::string>()) ||
#endif // TEMPERATURE_FAHRENHEIT
        !(doc["data"]["current_condition"][0]["weatherCode"].is<uint16_t>() ||
          doc["data"]["current_condition"][0]["weatherCode"].is<std::string>()))
    {
        queries.pop_back();
        lastMillis = millis() - interval + (1UL << 16U);
        ESP_LOGD(name, "unprocessable data");
        return;
    }
    WeatherHandler weather;
#if TEMPERATURE_FAHRENHEIT
    weather.temperature = round(doc["data"]["current_condition"][0]["temp_F"].as<float>());
#else
    weather.temperature = round(doc["data"]["current_condition"][0]["temp_C"].as<float>());
#endif // TEMPERATURE_FAHRENHEIT
    weather.parse(doc["data"]["current_condition"][0]["weatherCode"].as<uint16_t>(), codesets);
    weather.draw();
}

#endif // MODE_WORLDWEATHERONLINE
