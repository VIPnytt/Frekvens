#include "config/constants.h"

#if MODE_TICKER

#include <Preferences.h>

#include "extensions/HomeAssistantExtension.h"
#include "fonts/SmallFont.h"
#include "modes/TickerMode.h"
#include "services/DeviceService.h"
#include "services/ModesService.h"

void TickerMode::setup()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
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

    bool _transmit = false;
    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("message"))
    {
        message = Storage.getString("message").c_str();
        _transmit = true;
    }
    if (Storage.isKey("font"))
    {
        const String _font = Storage.getString("font");
        Storage.end();
        setFont(_font.c_str());
        _transmit = true;
    }
    else
    {
        Storage.end();
        setFont(FontSmall->name);
    }
    if (_transmit)
    {
        transmit();
    }
}

void TickerMode::wake()
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

void TickerMode::setMessage(std::string _message)
{
    if (_message != message)
    {
        message = _message;
        Preferences Storage;
        Storage.begin(name);
        Storage.putString("message", message.c_str());
        Storage.end();
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

void TickerMode::receiverHook(const JsonDocument doc, const char *const source)
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
        ESP_LOGD(source, "received");
    }
}

void TickerMode::sleep()
{
    text.reset();
}

#endif // MODE_TICKER
