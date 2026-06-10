#include "handlers/WeatherHandler.h"

#include "config/constants.h"             // NOLINT(misc-include-cleaner)
#include "services/ConnectivityService.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"      // NOLINT(misc-include-cleaner)

#include <esp_crt_bundle.h>  // NOLINT(misc-include-cleaner)
#include <esp_http_client.h> // NOLINT(misc-include-cleaner)
#include <variant>

int WeatherHandler::fetch(std::vector<char> &body, unsigned long &lastMillis)
{
    esp_http_client_config_t config{
        .host{host},
        .port{port},
        .path{path},
        .query{query},
        .user_agent{"Frekvens/" VERSION " (ESP32; +https://github.com/VIPnytt/Frekvens)"},
        .method{esp_http_client_method_t::HTTP_METHOD_GET},
        .transport_type{tls ? esp_http_client_transport_t::HTTP_TRANSPORT_OVER_SSL
                            : esp_http_client_transport_t::HTTP_TRANSPORT_OVER_TCP},
        .crt_bundle_attach{esp_crt_bundle_attach},
    };
    esp_http_client_handle_t client{esp_http_client_init(&config)};
    if (client == nullptr)
    {
        lastMillis = millis() - interval + UINT16_MAX;
        return -1;
    }
    for (std::pair<const char *const, const char *const> header : headers)
    {
        esp_http_client_set_header(client, header.first, header.second);
    }
    if (esp_http_client_open(client, 0) != ESP_OK || esp_http_client_fetch_headers(client) < 0)
    {
        esp_http_client_cleanup(client);
        lastMillis = millis() - interval + INT16_MAX;
        return -1;
    }
    const int status{esp_http_client_get_status_code(client)};
    if (status != 200)
    {
        esp_http_client_cleanup(client);
        ESP_LOGV("HTTP", "code %d", status);
        return status;
    }
    const int64_t length{esp_http_client_get_content_length(client)};
    if (length > 0)
    {
        body.reserve(static_cast<size_t>(length));
    }
    std::array<char, UINT8_MAX> buffer{};
    while (true)
    {
        const int read{esp_http_client_read(client, buffer.data(), static_cast<int>(buffer.size()))};
        if (read <= 0)
        {
            break;
        }
        body.insert(body.end(), buffer.data(), buffer.data() + read);
    }
    esp_http_client_cleanup(client);
    return status;
}

std::optional<WeatherHandler::Condition> WeatherHandler::getCondition(
    std::string_view code,
    std::span<const std::pair<WeatherHandler::Condition, std::span<const std::string_view>>> codesets)
{
    for (const std::pair<WeatherHandler::Condition, std::span<const std::string_view>> &codeset : codesets)
    {
        if (std::find(codeset.second.begin(), codeset.second.end(), code) != codeset.second.end())
        {
            return codeset.first;
        }
    }
    ESP_LOGD("Response", "unknown condition code %s", std::string(code).c_str());
    return std::nullopt;
}

template <typename T>
    requires std::is_unsigned_v<T>
std::optional<WeatherHandler::Condition>
WeatherHandler::getCondition(T code, std::span<const std::pair<WeatherHandler::Condition, std::span<const T>>> codesets)
{
    for (const std::pair<Condition, std::span<const T>> &codeset : codesets)
    {
        if (std::find(codeset.second.begin(), codeset.second.end(), code) != codeset.second.end())
        {
            return codeset.first;
        }
    }
    ESP_LOGD("Response", "unknown condition code %d", code);
    return std::nullopt;
}

template std::optional<WeatherHandler::Condition> WeatherHandler::getCondition<uint8_t>(
    uint8_t code, std::span<const std::pair<WeatherHandler::Condition, std::span<const uint8_t>>> codesets);
template std::optional<WeatherHandler::Condition> WeatherHandler::getCondition<uint16_t>(
    uint16_t code, std::span<const std::pair<WeatherHandler::Condition, std::span<const uint16_t>>> codesets);
template std::optional<WeatherHandler::Condition> WeatherHandler::getCondition<uint32_t>(
    uint32_t code, std::span<const std::pair<WeatherHandler::Condition, std::span<const uint32_t>>> codesets);

std::variant<std::span<const uint8_t>, std::span<const uint16_t>> WeatherHandler::getSign(Condition condition)
{
    // NOLINTBEGIN(bugprone-branch-clone)
    switch (condition)
    {
    case Condition::CLEAR:
#if PITCH_HORIZONTAL == PITCH_VERTICAL
        return bitmapClear;
#else
    {
        if (Display.getRatio() > 1.0F)
        {
            return bitmapClearTall;
        }
        return bitmapClearWide;
    }
#endif // PITCH_HORIZONTAL == PITCH_VERTICAL
    case Condition::CLOUDY:
        return bitmapCloudy;
    case Condition::CLOUDY_PARTLY:
        return bitmapCloudyPartly;
    case Condition::EXCEPTION:
        return bitmapExceptional;
    case Condition::FOG:
        return bitmapFog;
    case Condition::RAIN:
        return bitmapRain;
    case Condition::SNOW:
        return bitmapSnow;
    case Condition::THUNDER:
        return bitmapThunder;
    case Condition::WIND:
        return bitmapWind;
    }
    // NOLINTEND(bugprone-branch-clone)
    return {};
}
