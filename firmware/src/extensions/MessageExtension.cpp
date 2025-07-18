#include "config/constants.h"

#if EXTENSION_MESSAGE

#include <Preferences.h>

#include "extensions/HomeAssistantExtension.h"
#include "extensions/MessageExtension.h"
#include "extensions/MqttExtension.h"
#include "fonts/SmallFont.h"
#include "services/DeviceService.h"
#include "services/ModesService.h"

MessageExtension *Message = nullptr;

MessageExtension::MessageExtension() : ExtensionModule("Message")
{
    Message = this;
}

#if EXTENSION_HOMEASSISTANT
void MessageExtension::setup()
{
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_notify");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"message\":\"{{value}}\"}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::name] = "";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::platform] = "notify";
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
    }
}
#endif // EXTENSION_HOMEASSISTANT

void MessageExtension::ready()
{
    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("repeat"))
    {
        repeat = Storage.getUShort("repeat");
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
    pending = true;
}

void MessageExtension::handle()
{
    if (pending)
    {
        transmit();
        pending = false;
    }
    else if (Display.getPower() && millis() - lastMillis > INT8_MAX)
    {
        if (text && textWidth + offsetX < 0)
        {
            text.reset();
            messages.erase(messages.begin());
        }
        else if (text)
        {
            Display.clear();
            text->draw(offsetX, offsetY);
            --offsetX;
            lastMillis = millis();
        }
        else if (messages.size())
        {
            text = std::make_unique<TextHandler>(messages.front(), font);
            offsetX = COLUMNS;
            offsetY = (ROWS - text->getHeight()) / 2;
            textWidth = text->getWidth();
            if (!active)
            {
                Modes.set(false, name);
                Display.getFrame(frame);
                active = true;
            }
            lastMillis = millis();
            JsonDocument doc;
            doc["event"] = messages.front();
            Device.transmit(doc, name, false);
        }
        else if (active)
        {
            Display.setFrame(frame);
            Modes.set(true, name);
            active = false;
        }
    }
}

void MessageExtension::setFont(const char *const fontName)
{
    if (!font || strcmp(fontName, font->name))
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
        Serial.print(name);
        Serial.print(": unknown font ");
        Serial.println(fontName);
#endif
    }
}

void MessageExtension::setRepeat(uint8_t count)
{
    if (count != repeat)
    {
        repeat = count;
        Preferences Storage;
        Storage.begin(name);
        Storage.putUShort("repeat", repeat);
        Storage.end();
        pending = true;
    }
}

void MessageExtension::transmit()
{
    JsonDocument doc;
    doc["font"] = font->name;
    doc["repeat"] = repeat;
    Device.transmit(doc, name);
}

void MessageExtension::receiverHook(const JsonDocument doc)
{
    // Font
    if (doc["font"].is<const char *>())
    {
        setFont(doc["font"].as<const char *>());
    }
    // Repeat
    if (doc["repeat"].is<uint8_t>())
    {
        setRepeat(doc["repeat"].as<uint8_t>());
    }
    //  Message
    if (doc["message"].is<String>())
    {
        for (uint8_t _repeat = 0;; ++_repeat)
        {
            messages.push_back(doc["message"].as<String>());
            if (_repeat == repeat || _repeat == UINT8_MAX)
            {
                break;
            }
        }
#ifdef F_DEBUG
        Serial.print(name);
        Serial.println(": received");
#endif
    }
}

#endif // EXTENSION_MESSAGE
