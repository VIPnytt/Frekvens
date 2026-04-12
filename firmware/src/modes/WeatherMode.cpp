#if MODE_WEATHER

#include "modes/WeatherMode.h"

#include "extensions/HomeAssistantExtension.h"
#include "fonts/MiniFont.h" // NOLINT(misc-include-cleaner)
#include "handlers/BitmapHandler.h"
#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

#include <Preferences.h>
#include <WiFi.h> // NOLINT(misc-include-cleaner)

void WeatherMode::configure()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic{std::string("frekvens/" HOSTNAME "/").append(name)};
    {
        const std::string id{std::string(name).append("_protocol")};
        JsonObject component{(*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"provider":"{{value}}"})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:weather-partly-cloudy");
        component[HomeAssistantAbbreviations::name].set(std::string(name).append(" provider"));
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        JsonArray options{component[HomeAssistantAbbreviations::options].to<JsonArray>()};
        for (const std::string_view _provider : providerNames)
        {
            options.add(_provider);
        }
        component[HomeAssistantAbbreviations::platform].set("select");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(HomeAssistant->uniquePrefix + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.provider}}");
    }
#endif // EXTENSION_HOMEASSISTANT
    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("provider"))
    {
        const String _provider = Storage.getString("provider");
        Storage.end();
        setProvider(_provider.c_str());
    }
    else
    {
        Storage.end();
    }
    if (provider == nullptr)
    {
        setProvider(providerNames.front());
    }
}

void WeatherMode::begin() { lastMillis = millis() - provider->interval; }

void WeatherMode::handle()
{
    if (WiFi.isConnected() && millis() - lastMillis > provider->interval)
    {
        condition.reset();
        temperature.reset();
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
                    // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
                    const uint8_t marginsY{
                        static_cast<uint8_t>(std::max(0, GRID_ROWS - bitmap.getHeight() - textHeight) / 3)};
                    Display.clearFrame();
                    bitmap.draw((GRID_COLUMNS - bitmap.getWidth()) / 2, marginsY);
                    text.draw((GRID_COLUMNS - text.getWidth()) / 2, GRID_ROWS - marginsY - textHeight);
                },
                provider->getSign(condition.value()));
            transmit();
        }
    }
}

void WeatherMode::setProvider(std::string_view providerName)
{
    if (provider == nullptr || provider->name != providerName)
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
            Preferences Storage;
            Storage.begin(name);
            Storage.putString("provider", provider->name.data());
            Storage.end();
            condition.reset();
            temperature.reset();
            lastMillis = millis() - provider->interval;
            transmit();
        }
    }
}

void WeatherMode::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
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

void WeatherMode::onReceive(JsonObjectConst payload,
                            const char *source) // NOLINT(misc-unused-parameters)
{
    // Provider
    if (payload["provider"].is<std::string_view>())
    {
        setProvider(payload["provider"].as<std::string_view>());
    }
}

void WeatherMode::end()
{
    condition.reset();
    temperature.reset();
    transmit();
}

#endif // MODE_WEATHER
