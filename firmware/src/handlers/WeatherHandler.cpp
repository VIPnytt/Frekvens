#include "handlers/WeatherHandler.h"

#include "config/constants.h"             // NOLINT(misc-include-cleaner)
#include "services/ConnectivityService.h" // NOLINT(misc-include-cleaner)

#include <esp_crt_bundle.h>  // NOLINT(misc-include-cleaner)
#include <esp_http_client.h> // NOLINT(misc-include-cleaner)

void WeatherHandler::update(std::optional<Conditions> &condition, std::optional<int16_t> &temperature,
                            unsigned long &lastMillis)
{
}

int WeatherHandler::fetch(std::vector<char> &body, unsigned long &lastMillis)
{
    esp_http_client_config_t config = {
        .host = host,
        .port = port,
        .path = path,
        .query = query,
        .user_agent = Connectivity.userAgent.data(),
        .method = esp_http_client_method_t::HTTP_METHOD_GET,
        .transport_type = tls ? esp_http_client_transport_t::HTTP_TRANSPORT_OVER_SSL
                              : esp_http_client_transport_t::HTTP_TRANSPORT_OVER_TCP,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == nullptr)
    {
        lastMillis = millis() - interval + UINT16_MAX;
        return -1;
    }
    for (std::pair<std::string_view, std::string_view> header : headers)
    {
        esp_http_client_set_header(client, header.first.data(), header.second.data());
    }
    if (esp_http_client_open(client, 0) != ESP_OK || esp_http_client_fetch_headers(client) < 0)
    {
        esp_http_client_cleanup(client);
        lastMillis = millis() - interval + INT16_MAX;
        return -1;
    }
    const int status = esp_http_client_get_status_code(client);
    if (status != 200)
    {
        esp_http_client_cleanup(client);
        ESP_LOGV(_name, "HTTP %d", status);
        return status;
    }
    const int64_t len = esp_http_client_get_content_length(client);
    if (len > 0)
    {
        body.reserve(static_cast<size_t>(len));
    }
    std::array<char, UINT8_MAX> buffer{};
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
    return status;
}

std::optional<WeatherHandler::Conditions> WeatherHandler::getCondition(std::string_view code,
                                                                       std::span<const Codeset> codesets)
{
    for (const Codeset &codeset : codesets)
    {
        if (std::find(codeset.codes.begin(), codeset.codes.end(), code) != codeset.codes.end())
        {
            return codeset.condition;
        }
    }
    ESP_LOGD(_name.data(), "unknown condition code %s", code.data());
    return std::nullopt;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
std::optional<WeatherHandler::Conditions> WeatherHandler::getCondition(uint8_t code, std::span<const Codeset8> codesets)
{
    for (const Codeset8 &codeset : codesets)
    {
        if (std::find(codeset.codes.begin(), codeset.codes.end(), code) != codeset.codes.end())
        {
            return codeset.condition;
        }
    }
    ESP_LOGD(_name.data(), "unknown condition code %d", code);
    return std::nullopt;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
std::optional<WeatherHandler::Conditions> WeatherHandler::getCondition(uint16_t code,
                                                                       std::span<const Codeset16> codesets)
{
    for (const Codeset16 &codeset : codesets)
    {
        if (std::find(codeset.codes.begin(), codeset.codes.end(), code) != codeset.codes.end())
        {
            return codeset.condition;
        }
    }
    ESP_LOGD(_name.data(), "unknown condition code %d", code);
    return std::nullopt;
}

std::span<const uint16_t> WeatherHandler::getSign(Conditions condition)
{
    switch (condition)
    {
    case Conditions::CLEAR:
#if PITCH_HORIZONTAL == PITCH_VERTICAL
        return conditionClear;
#else
    {
        if (Display.getRatio() > 1.0F)
        {
            return conditionClearTall
        }
        return conditionClearWide;
    }
#endif // PITCH_HORIZONTAL == PITCH_VERTICAL
    case Conditions::CLOUDY:
        return conditionCloudy;
    case Conditions::CLOUDY_PARTLY:
        return conditionCloudyPartly;
    case Conditions::EXCEPTION:
        return conditionExceptional;
    case Conditions::FOG:
        return conditionFog;
    case Conditions::RAIN:
        return conditionRain;
    case Conditions::SNOW:
        return conditionSnow;
    case Conditions::THUNDER:
        return conditionThunder;
    case Conditions::WIND:
        return conditionWind;
    }
    return {};
}
