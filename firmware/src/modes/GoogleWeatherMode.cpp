#if MODE_GOOGLEWEATHER

#include "modes/GoogleWeatherMode.h"

#include "services/ConnectivityService.h" // NOLINT(misc-include-cleaner)

#include <esp_crt_bundle.h> // NOLINT(misc-include-cleaner)
#include <esp_http_client.h>

void GoogleWeatherMode::begin() { lastMillis = millis() - interval; }

void GoogleWeatherMode::handle()
{
    if (WiFi.isConnected() && millis() - lastMillis >= interval)
    {
        update();
    }
}

void GoogleWeatherMode::update()
{
    lastMillis = millis();
    if (queries.empty())
    {
        ESP_LOGE(name, "unable to fetch weather"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        return;
    }
    esp_http_client_config_t config = {
        .host = "weather.googleapis.com",
        .port = 443,
        .path = "/v1/currentConditions:lookup",
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
            queries.pop_back();
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
    filter["temperature"]["degrees"].set(true);
    filter["weatherCondition"]["type"].set(true);
    JsonDocument doc; // NOLINT(misc-const-correctness)
    if (deserializeJson(doc, body.data(), DeserializationOption::Filter(filter)) != DeserializationError::Code::Ok ||
        !doc["temperature"]["degrees"].is<float>() || !doc["weatherCondition"]["type"].is<std::string>())
    {
        queries.pop_back();
        lastMillis = millis() - interval + (1UL << 16U);
        ESP_LOGD(name, "unprocessable data"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        return;
    }
    WeatherHandler weather;
    weather.temperature = round(doc["temperature"]["degrees"].as<float>());
    weather.parse(doc["weatherCondition"]["type"].as<std::string>(), codesets);
    weather.draw();
}

#endif // MODE_GOOGLEWEATHER
