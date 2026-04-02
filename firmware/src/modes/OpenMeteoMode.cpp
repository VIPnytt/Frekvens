#if MODE_OPENMETEO

#include "modes/OpenMeteoMode.h"

#include "services/ConnectivityService.h" // NOLINT(misc-include-cleaner)

#include <esp_crt_bundle.h> // NOLINT(misc-include-cleaner)
#include <esp_http_client.h>

void OpenMeteoMode::begin() { lastMillis = millis() - interval; }

void OpenMeteoMode::handle()
{
    if (WiFi.isConnected() && millis() - lastMillis >= interval)
    {
        update();
    }
}

void OpenMeteoMode::update()
{
    lastMillis = millis();
    if (parts.empty())
    {
        ESP_LOGE(name, "unable to fetch weather"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        return;
    }
    esp_http_client_config_t config = {
        .host = parts.back().first,
        .port = 443,
        .path = "/v1/forecast",
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
            ESP_LOGV(name, "http status %d", status); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
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
    filter["current"]["temperature_2m"].set(true);
    filter["current"]["weather_code"].set(true);
    JsonDocument doc; // NOLINT(misc-const-correctness)
    if (deserializeJson(doc, body.data(), DeserializationOption::Filter(filter)) != DeserializationError::Code::Ok ||
        !doc["current"]["temperature_2m"].is<float>() || !doc["current"]["weather_code"].is<uint8_t>())
    {
        parts.pop_back();
        lastMillis = millis() - interval + (1UL << 16U);
        ESP_LOGD(name, "unprocessable data"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        return;
    }
    WeatherHandler weather;
    weather.temperature = round(doc["current"]["temperature_2m"].as<float>());
    weather.parse(doc["current"]["weather_code"].as<uint8_t>(), codesets);
    weather.draw();
}

#endif // MODE_OPENMETEO
