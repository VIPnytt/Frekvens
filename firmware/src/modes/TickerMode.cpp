#if MODE_TICKER

#include "modes/TickerMode.h"

#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "fonts/SmallFont.h"                   // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)
#include "services/FontsService.h"      // NOLINT(misc-include-cleaner)

#include <nvs.h>

/**
 * @brief Loads the persisted ticker message and font, applies a fallback font when needed, and transmits the resulting
 * configuration.
 */
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
            size_t length{_fontName.size()};
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
        setFont(FontsService::names[0U]);
#endif // FONT_SMALL
    }
    transmit();
}

/**
 * @brief Prepares the ticker for initialization using its persisted or default font.
 */
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
        setFont(FontsService::names[0U]);
#endif // FONT_SMALL
    }
    pending = true;
}

/**
 * @brief Updates and renders the scrolling ticker text.
 *
 * Initializes pending text, advances its position at timed intervals, and
 * restarts the scroll cycle after the text leaves the display. When microphone
 * support is enabled, a microphone trigger is required to begin each cycle.
 */
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
        Display.fillFrame(0U);
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

/**
 * @brief Transmits the current ticker font and message configuration.
 */
void TickerMode::transmit()
{
    JsonDocument doc{};
    doc["font"].set(font->name);
    doc["message"].set(message);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

/**
 * @brief Applies font and message settings received in a JSON payload.
 *
 * @param payload JSON object containing optional `font` and `message` string properties.
 * @param source Origin of the received payload.
 */
void TickerMode::onReceive(JsonObjectConst payload, std::string_view source)
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

/**
 * @brief Releases the active text handler.
 */
void TickerMode::end() { text.reset(); }

#if EXTENSION_HOMEASSISTANT
/**
 * @brief Configures Home Assistant discovery entities for ticker font selection and message editing.
 *
 * @param discovery Home Assistant discovery document to update.
 * @param topic Base topic for ticker state and commands.
 * @param unique Unique identifier prefix for the discovered entities.
 */
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
        for (const std::string_view _font : FontsService::names)
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
