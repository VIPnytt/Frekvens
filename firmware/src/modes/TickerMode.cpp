#if MODE_TICKER

#include "modes/TickerMode.h"

#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "fonts/SmallFont.h"                   // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)
#include "services/FontsService.h"      // NOLINT(misc-include-cleaner)

#include <nvs.h>

void TickerMode::configure()
{
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        {
            size_t length{0U};
            if (nvs_get_str(handle, "message", nullptr, &length) == ESP_OK && length > 1U)
            {
                std::string _message(length, '\0');
                if (nvs_get_str(handle, "message", _message.data(), &length) == ESP_OK)
                {
                    _message.resize(length - 1U);
                    message = std::move(_message);
                }
            }
        }
        {
            std::array<char, FontsService::namesMaxLength + 1U> _fontName{};
            size_t length{_fontName.size()}; // NOLINT(cppcoreguidelines-init-variables)
            if (nvs_get_str(handle, "font", _fontName.data(), &length) == ESP_OK)
            {
                setFont({_fontName.data(), length - 1U});
            }
        }
        nvs_close(handle);
    }
    if (!font)
    {
#if FONT_SMALL
        setFont(SmallFont::name);
#else
        setFont(Fonts.names[0U]);
#endif // FONT_SMALL
    }
    transmit();
}

void TickerMode::begin()
{
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        std::array<char, FontsService::namesMaxLength + 1U> _fontName{};
        size_t length{_fontName.size()};
        if (nvs_get_str(handle, "font", _fontName.data(), &length) == ESP_OK)
        {
            setFont({_fontName.data(), length - 1U});
        }
        nvs_close(handle);
    }
    if (!font)
    {
#if FONT_SMALL
        setFont(SmallFont::name);
#else
        setFont(Fonts.names[0U]);
#endif // FONT_SMALL
    }
    pending = true;
}

void TickerMode::handle()
{
    if (pending)
    {
        text = std::make_unique<TextHandler>(message, *font);
        offsetX = GRID_COLUMNS;
        offsetY = (GRID_ROWS - text->getHeight()) / 2U;
        width = text->getWidth();
        transmit();
        pending = false;
    }
    else if (text && millis() - lastMillis > INT8_MAX)
    {
        if (width + offsetX < 0)
        {
            offsetX = GRID_COLUMNS;
        }
        lastMillis = millis();
#if EXTENSION_MICROPHONE
        if (offsetX == GRID_COLUMNS && !Extensions.Microphone().isTriggered())
        {
            return;
        }
#endif // EXTENSION_MICROPHONE
        Display.clearFrame();
        text->draw(offsetX, offsetY);
        --offsetX;
    }
}

void TickerMode::setFont(std::string_view fontName)
{
    if (std::ranges::find(FontsService::names, fontName) != FontsService::names.end())
    {
        font = Fonts.get(fontName);
        nvs_handle_t handle{};
        if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
        {
            nvs_set_str(handle, "font", font->name.data());
            nvs_commit(handle);
            nvs_close(handle);
        }
        pending = true;
    }
}

void TickerMode::setMessage(std::string_view _message)
{
    if (_message.length())
    {
        message = _message;
        nvs_handle_t handle{};
        if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
        {
            nvs_set_str(handle, "message", message.c_str());
            nvs_commit(handle);
            nvs_close(handle);
        }
        pending = true;
    }
}

void TickerMode::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["font"].set(font->name);
    doc["message"].set(message);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void TickerMode::onReceive(JsonObjectConst payload,
                           std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Font
    if (payload["font"].is<std::string_view>())
    {
        setFont(payload["font"].as<std::string_view>());
    }
    //  Message
    if (payload["message"].is<std::string_view>())
    {
        setMessage(payload["message"].as<std::string_view>());
    }
}

void TickerMode::end() { text.reset(); }

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void TickerMode::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_font")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"font":"{{value}}"})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:format-font");
        component[HomeAssistantAbbreviations::name].set(std::string(name).append(" font"));
        JsonArray options{component[HomeAssistantAbbreviations::options].to<JsonArray>()};
        for (const std::string_view _font : Fonts.names)
        {
            options.add(_font);
        }
        component[HomeAssistantAbbreviations::platform].set("select");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.font}}");
    }
    {
        const std::string id{std::string(name).append("_message")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"message":"{{value}}"})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::icon].set("mdi:message");
        component[HomeAssistantAbbreviations::name].set(name);
        component[HomeAssistantAbbreviations::platform].set("text");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.message}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // MODE_TICKER
