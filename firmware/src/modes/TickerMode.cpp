#include <Preferences.h>

#include "config/constants.h"
#include "extensions/HomeAssistantExtension.h"
#include "fonts/MiniFont.h"
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
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"message\":\"{{value}}\"}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::icon] = "mdi:message";
        component[Abbreviations::name] = name;
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::platform] = "text";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::value_template] = "{{value_json.message}}";
    }
#endif // EXTENSION_HOMEASSISTANT

    bool _transmit = false;
    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("message"))
    {
        message = Storage.getString("message");
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
    }
    if (!font)
    {
#if defined(FONT_SMALL) && !FONT_SMALL
        font = FontMini;
#else
        font = FontSmall;
#endif // defined(FONT_SMALL) && !FONT_SMALL
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
    if (pending)
    {
        if (message.length())
        {
            text = std::make_unique<TextHandler>(message, font);
            offsetX = COLUMNS;
            offsetY = (ROWS - text->getHeight()) / 2;
            textWidth = text->getWidth();
        }
        else if (text)
        {
            text.reset();
            Display.clear();
        }
        transmit();
        pending = false;
    }
    else if (text && millis() - lastMillis > INT8_MAX)
    {
        if (textWidth + offsetX < 0)
        {
            offsetX = COLUMNS;
        }
        Display.clear();
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
#ifdef F_DEBUG
    Serial.printf("%s: unknown font %s\n", name, fontName);
#endif
}

void TickerMode::setMessage(String textMessage)
{
    if (!textMessage.equals(message))
    {
        message = textMessage;
        Preferences Storage;
        Storage.begin(name);
        Storage.putString("message", message);
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

void TickerMode::receiverHook(const JsonDocument doc)
{
    // Font
    if (doc["font"].is<const char *>())
    {
        setFont(doc["font"].as<const char *>());
    }
    //  Message
    if (doc["message"].is<String>())
    {
        setMessage(doc["message"].as<String>());
#ifdef F_DEBUG
        Serial.printf("%s: received\n", name);
#endif
    }
}

void TickerMode::sleep()
{
    text.reset();
}
