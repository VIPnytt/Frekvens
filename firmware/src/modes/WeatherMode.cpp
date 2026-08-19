#if MODE_WEATHER

#include "modes/WeatherMode.h"

#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "fonts/MiniFont.h"                    // NOLINT(misc-include-cleaner)
#include "handlers/BitmapHandler.h"            // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h"              // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)

#include <WiFi.h> // NOLINT(misc-include-cleaner)
#include <nvs.h>

static_assert(GRID_COLUMNS >= 14U, __STRING(MODE_WEATHER) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 14U, __STRING(MODE_WEATHER) " is not compatible with this device's display size.");

/**
 * @brief Configures the weather provider from persisted settings.
 *
 * Selects the persisted provider when available, otherwise selects the first
 * provider, then publishes the current weather mode state.
 */
void WeatherMode::configure()
{
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        std::array<char, providerNamesMaxLength + 1U> _providerName{};
        size_t length{_providerName.size()};
        if (nvs_get_str(handle, "provider", _providerName.data(), &length) == ESP_OK)
        {
            setProvider({_providerName.data(), length - 1U});
        }
        nvs_close(handle);
    }
    if (provider == nullptr)
    {
        setProvider(providerNames[0U]);
    }
    transmit();
}

/**
 * @brief Initializes the weather provider from persistent configuration.
 *
 * Uses the first available provider when no valid provider is configured and
 * schedules the selected provider for an immediate update.
 */
void WeatherMode::begin()
{
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        size_t length{0U};
        if (nvs_get_str(handle, "provider", nullptr, &length) == ESP_OK && length > 1U)
        {
            std::array<char, providerNamesMaxLength + 1U> _providerName{};
            size_t length{_providerName.size()};
            if (nvs_get_str(handle, "provider", _providerName.data(), &length) == ESP_OK)
            {
                setProvider({_providerName.data(), length - 1U});
            }
            nvs_close(handle);
        }
    }
    if (provider == nullptr)
    {
        setProvider(providerNames[0U]);
    }
    lastMillis = millis() - provider->interval;
}

/**
 * @brief Updates weather data and renders the current condition and temperature.
 *
 * Refreshes the provider data after the configured interval when Wi-Fi is connected.
 * When both values are available, displays the condition above the centered temperature
 * and publishes the updated state.
 */
void WeatherMode::handle()
{
    if (WiFi.isConnected() && millis() - lastMillis > provider->interval)
    {
        lastMillis = millis();
        provider->update(condition, temperature, lastMillis);
        if (condition.has_value() && temperature.has_value())
        {
            std::visit(
                [&](auto sign)
                {
                    const BitmapHandler bitmap(sign);
                    const MiniFont font;
                    const TextHandler text(std::to_string(temperature.value()) + "°", font);
                    const uint8_t textHeight{text.getHeight()};
                    const uint8_t marginsY{
                        static_cast<uint8_t>(max(0U, GRID_ROWS - bitmap.getHeight() - textHeight) / 3U)};
                    Display.fillFrame(0U);
                    bitmap.draw((GRID_COLUMNS - bitmap.getWidth()) / 2U, marginsY);
                    text.draw((GRID_COLUMNS - text.getWidth()) / 2U, GRID_ROWS - marginsY - textHeight);
                },
                provider->getSign(condition.value()));
            transmit();
        }
    }
}

void WeatherMode::setProvider(std::string_view providerName)
{
#if WEATHER_GOOGLE
    if (providerName == GoogleWeatherMiddleware::name)
    {
        provider = std::make_unique<GoogleWeatherMiddleware>();
    }
#endif // WEATHER_GOOGLE
#if WEATHER_HOMEASSISTANT
    if (providerName == HomeAssistantWeatherMiddleware::name)
    {
        provider = std::make_unique<HomeAssistantWeatherMiddleware>();
    }
#endif // WEATHER_HOMEASSISTANT
#if WEATHER_OPENMETEO
    if (providerName == OpenMeteoMiddleware::name)
    {
        provider = std::make_unique<OpenMeteoMiddleware>();
    }
#endif // WEATHER_OPENMETEO
#if WEATHER_OPENWEATHER
    if (providerName == OpenWeatherMiddleware::name)
    {
        provider = std::make_unique<OpenWeatherMiddleware>();
    }
#endif // WEATHER_OPENWEATHER
#if WEATHER_TOMORROWIO
    if (providerName == TomorrowIoMiddleware::name)
    {
        provider = std::make_unique<TomorrowIoMiddleware>();
    }
#endif // WEATHER_TOMORROWIO
#if WEATHER_WORLDWEATHERONLINE
    if (providerName == WorldWeatherOnlineMiddleware::name)
    {
        provider = std::make_unique<WorldWeatherOnlineMiddleware>();
    }
#endif // WEATHER_WORLDWEATHERONLINE
#if WEATHER_WTTRIN
    if (providerName == WttrInMiddleware::name)
    {
        provider = std::make_unique<WttrInMiddleware>();
    }
#endif // WEATHER_WTTRIN
#if WEATHER_YR
    if (providerName == YrMiddleware::name)
    {
        provider = std::make_unique<YrMiddleware>();
    }
#endif // WEATHER_YR
    if (provider)
    {
        nvs_handle_t handle{};
        if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
        {
            nvs_set_str(handle, "provider", provider->name.data());
            nvs_commit(handle);
            nvs_close(handle);
        }
        condition.reset();
        temperature.reset();
        lastMillis = millis() - provider->interval;
        transmit();
    }
}

/**
 * @brief Publishes the current weather state and available providers.
 */
void WeatherMode::transmit()
{
    JsonDocument doc{};
    if (condition.has_value())
    {
        doc["condition"].set(conditionNames[static_cast<size_t>(condition.value())]);
    }
    doc["provider"].set(provider->name);
    JsonArray _providers{doc["providers"].to<JsonArray>()};
    for (const std::string_view _provider : providerNames)
    {
        _providers.add(_provider);
    }
    if (temperature.has_value())
    {
        doc["temperature"].set(temperature.value());
    }
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

/**
 * @brief Handles incoming provider selection requests.
 *
 * @param payload Message payload containing an optional string-valued `provider` field.
 * @param source Message source.
 */
void WeatherMode::onReceive(JsonObjectConst payload, std::string_view source)
{
    // Provider
    if (payload["provider"].is<std::string_view>())
    {
        setProvider(payload["provider"].as<std::string_view>());
    }
}

#if EXTENSION_HOMEASSISTANT
/**
 * @brief Registers the weather provider selector with Home Assistant.
 *
 * @param discovery Home Assistant discovery document to update.
 * @param topic Base topic for the selector's command and state messages.
 * @param unique Prefix used to construct the selector's unique identifier.
 */
void WeatherMode::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_protocol")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"provider":"{{value}}"})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:weather-partly-cloudy");
        component[HomeAssistantAbbreviations::name].set(std::string(name).append(" provider"));
        JsonArray options{component[HomeAssistantAbbreviations::options].to<JsonArray>()};
        for (const std::string_view _provider : providerNames)
        {
            options.add(_provider);
        }
        component[HomeAssistantAbbreviations::platform].set("select");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.provider}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // MODE_WEATHER
