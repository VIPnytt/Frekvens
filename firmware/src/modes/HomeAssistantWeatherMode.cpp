#if MODE_HOMEASSISTANTWEATHER

#include "modes/HomeAssistantWeatherMode.h"

#include "services/ConnectivityService.h"

#include <esp_crt_bundle.h>
#include <esp_http_client.h>

void HomeAssistantWeatherMode::begin() { lastMillis = millis() - interval; }

void HomeAssistantWeatherMode::handle()
{
    if (WiFi.isConnected() && millis() - lastMillis >= interval)
    {
        update();
    }
}

void HomeAssistantWeatherMode::update()
{
    lastMillis = millis();
    if (paths.empty())
    {
        ESP_LOGE(name, "unable to fetch weather");
        return;
    }
    esp_http_client_config_t config = {
#ifdef HOMEASSISTANT_HOST
        .host = HOMEASSISTANT_HOST,
#else
        .host = "homeassistant.local",
#endif // HOMEASSISTANT_HOST
#ifdef HOMEASSISTANT_PORT
        .port = HOMEASSISTANT_PORT,
#else
        .port = 8123,
#endif // HOMEASSISTANT_PORT
        .path = paths.back(),
        .user_agent = Connectivity.userAgent.data(),
        .method = esp_http_client_method_t::HTTP_METHOD_GET,
#ifdef HOMEASSISTANT_PROTOCOL
        .transport_type = strcmp(HOMEASSISTANT_PROTOCOL, "http:")
                              ? esp_http_client_transport_t::HTTP_TRANSPORT_OVER_SSL
                              : esp_http_client_transport_t::HTTP_TRANSPORT_OVER_TCP,
        .crt_bundle_attach = esp_crt_bundle_attach,
#endif // HOMEASSISTANT_PROTOCOL
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == nullptr)
    {
        lastMillis = millis() - interval + (1UL << 15U);
        return;
    }
    esp_http_client_set_header(client, "Accept", "application/json");
    esp_http_client_set_header(client, "Authorization", "Bearer " HOMEASSISTANT_KEY);
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
            paths.pop_back();
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
    filter["attributes"]["temperature"].set(true);
    filter["state"].set(true);
    JsonDocument doc; // NOLINT(misc-const-correctness)
    if (deserializeJson(doc, body.data(), DeserializationOption::Filter(filter)) != DeserializationError::Code::Ok ||
        !doc["attributes"]["temperature"].is<float>() || !doc["state"].is<std::string>())
    {
        paths.pop_back();
        lastMillis = millis() - interval + (1UL << 16U);
        ESP_LOGD(name, "unprocessable data");
        return;
    }
    WeatherHandler weather;
    weather.temperature = round(doc["attributes"]["temperature"].as<float>());
    weather.parse(doc["state"].as<std::string>(), codesets);
    weather.draw();
}

#endif // MODE_HOMEASSISTANTWEATHER
