#if MODE_TICKER

#include <Preferences.h>

#include "extensions/HomeAssistantExtension.h"
#include "fonts/SmallFont.h"
#include "modes/TickerMode.h"
#include "services/DeviceService.h"
#include "services/ModesService.h"

void TickerMode::configure()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_font");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"font\":\"{{value}}\"}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::enabled_by_default] = false;
        component[HomeAssistantAbbreviations::entity_category] = "config";
        component[HomeAssistantAbbreviations::icon] = "mdi:format-font";
        component[HomeAssistantAbbreviations::name] = std::string(name).append(" font");
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        JsonArray options = component[HomeAssistantAbbreviations::options].to<JsonArray>();
        for (const FontModule *_font : Fonts.getAll())
        {
            options.add(_font->name);
        }
        component[HomeAssistantAbbreviations::platform] = "select";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.font}}";
    }
    {
        const std::string id = std::string(name).append("_message");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"message\":\"{{value}}\"}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::icon] = "mdi:message";
        component[HomeAssistantAbbreviations::name] = name;
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::platform] = "text";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.message}}";
    }
#endif // EXTENSION_HOMEASSISTANT
    Preferences Storage;
    Storage.begin(name, true);
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
        font = FontSmall;
    }
    transmit();
}

void TickerMode::begin()
{
    pending = true;
}

void TickerMode::handle()
{
    if (pending && message.length())
    {
        text = std::make_unique<TextHandler>(message, font);
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

void TickerMode::setFont(const char *const fontName)
{
    if (!font || strcmp(font->name, fontName))
    {
        for (FontModule *_font : Fonts.getAll())
        {
            if (!strcmp(_font->name, fontName))
            {
                font = _font;
                Preferences Storage;
                Storage.begin(name);
                Storage.putString("font", font->name);
                Storage.end();
                pending = true;
                return;
            }
        }
        ESP_LOGD(name, "unknown font %s", fontName);
    }
}

void TickerMode::setMessage(std::string _message)
{
    if (_message != message)
    {
        message = _message;
        Preferences Storage;
        Storage.begin(name);
        Storage.putString("message", message.c_str());
        Storage.end();
        ESP_LOGD(name, "received");
        pending = true;
    }
}

void TickerMode::transmit()
{
    JsonDocument doc;
    doc["font"] = font->name;
    doc["message"] = message;
    Device.transmit(doc, name);
}

void TickerMode::onReceive(const JsonDocument doc, const char *const source)
{
    // Font
    if (doc["font"].is<const char *>())
    {
        setFont(doc["font"].as<const char *>());
    }
    //  Message
    if (doc["message"].is<std::string>())
    {
        setMessage(doc["message"].as<std::string>());
    }
}

void TickerMode::end()
{
    text.reset();
}

#endif // MODE_TICKER
