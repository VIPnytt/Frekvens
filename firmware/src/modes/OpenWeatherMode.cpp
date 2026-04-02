#if MODE_OPENWEATHER

#include "modes/OpenWeatherMode.h"

#include "services/ConnectivityService.h" // NOLINT(misc-include-cleaner)

#include <esp_crt_bundle.h> // NOLINT(misc-include-cleaner)
#include <esp_http_client.h>

void OpenWeatherMode::begin() { lastMillis = millis() - interval; }

void OpenWeatherMode::handle()
{
    if (WiFi.isConnected() && millis() - lastMillis >= interval)
    {
        update();
    }
}

void OpenWeatherMode::update()
{
    lastMillis = millis();
    if (parts.empty())
    {
        ESP_LOGE(name, "unable to fetch weather"); // NOLINT(cppcoreguidelines-pro-type-vararg)
        return;
    }
    esp_http_client_config_t config = {
        .host = "api.openweathermap.org",
        .port = 443,
        .path = parts.back().first,
        .query = parts.back().second,
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
    if (esp_http_client_open(client, 0) != ESP_OK || esp_http_client_fetch_headers(client) < 0)
    {
        esp_http_client_cleanup(client);
        lastMillis = millis() - interval + (1UL << 14U);
        return;
    }
    const int status = esp_http_client_get_status_code(client);
    if (status != 200)
    {
        esp_http_client_cleanup(client);
        if (status >= 400 && status < 500)
        {
            parts.pop_back();
            lastMillis = millis() - interval + (1UL << 13U);
            ESP_LOGV(name, "http status %d", status); // NOLINT(cppcoreguidelines-pro-type-vararg)
        }
        return;
    }
    std::vector<char> body;
    const int64_t len = esp_http_client_get_content_length(client);
    if (len > 0)
    {
        body.reserve(static_cast<size_t>(len) + 1U);
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
    esp_http_client_cleanup(client);
    body.push_back('\0');
    JsonDocument filter; // NOLINT(misc-const-correctness)
    filter["current"]["temp"].set(true);
    filter["current"]["weather"]["id"].set(true);
    filter["main"]["temp"].set(true);
    filter["weather"][0]["id"].set(true);
    JsonDocument doc; // NOLINT(misc-const-correctness)
    const bool deserialized =
        deserializeJson(doc, body.data(), DeserializationOption::Filter(filter)) == DeserializationError::Code::Ok;
    if (deserialized && doc["main"]["temp"].is<float>() && doc["weather"][0]["id"].is<uint16_t>())
    {
        // API 2.5
        WeatherHandler weather;
        weather.temperature = round(doc["main"]["temp"].as<float>());
        weather.parse(doc["weather"][0]["id"].as<uint16_t>(), codesets);
        weather.draw();
        return;
    }
    if (deserialized && doc["current"]["temp"].is<float>() && doc["current"]["weather"]["id"].is<uint16_t>())
    {
        // API 3.0
        WeatherHandler weather;
        weather.temperature = round(doc["current"]["temp"].as<float>());
        weather.parse(doc["current"]["weather"]["id"].as<uint16_t>(), codesets);
        weather.draw();
        return;
    }
    parts.pop_back();
    lastMillis = millis() - interval + (1UL << 16U);
    ESP_LOGD(name, "unprocessable data"); // NOLINT(cppcoreguidelines-pro-type-vararg)
}

#endif // MODE_OPENWEATHER
