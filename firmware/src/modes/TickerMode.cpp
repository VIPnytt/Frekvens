#if MODE_TICKER

#include "modes/TickerMode.h"

#include "extensions/HomeAssistantExtension.h"
#include "fonts/SmallFont.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"
#include "services/FontsService.h" // NOLINT(misc-include-cleaner)

#include <Preferences.h>

void TickerMode::configure()
{
    Preferences Storage;
    Storage.begin(name.data(), true);
    if (Storage.isKey("message"))
    {
        message = Storage.getString("message").c_str();
    }
    if (Storage.isKey("font"))
    {
        const String _font = Storage.getString("font");
        Storage.end();
        setFont(_font.c_str());
    }
    else
    {
        Storage.end();
    }
    if (!font)
    {
#if FONT_SMALL
        setFont(SmallFont::name);
#else
        setFont(Fonts.names[0]);
#endif // FONT_SMALL
    }
    transmit();
}

void TickerMode::begin() { pending = true; }

void TickerMode::handle()
{
    if (pending)
    {
        text = std::make_unique<TextHandler>(message, *font);
        offsetX = GRID_COLUMNS;
        offsetY = (GRID_ROWS - text->getHeight()) / 2;
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
        Display.clearFrame();
        text->draw(offsetX, offsetY);
        --offsetX;
        lastMillis = millis();
    }
}

void TickerMode::setFont(std::string_view fontName)
{
    if (std::unique_ptr<const FontModule> _font = Fonts.get(fontName))
    {
        font = std::move(_font);
        Preferences Storage;
        Storage.begin(name.data());
        Storage.putString("font", font->name.data());
        Storage.end();
        pending = true;
    }
}

void TickerMode::setMessage(std::string_view _message)
{
    if (_message.length())
    {
        message = _message.data();
        Preferences Storage;
        Storage.begin(name.data());
        Storage.putString("message", message.c_str());
        Storage.end();
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
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
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
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::platform].set("text");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.message}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // MODE_TICKER
