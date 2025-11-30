#include "config/constants.h"

#if EXTENSION_MESSAGE

#include <Preferences.h>

#include "extensions/HomeAssistantExtension.h"
#include "extensions/MessageExtension.h"
#include "fonts/SmallFont.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ModesService.h"

MessageExtension *Message = nullptr;

MessageExtension::MessageExtension() : ExtensionModule("Message")
{
    Message = this;
}

#if EXTENSION_HOMEASSISTANT
void MessageExtension::configure()
{
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_notify");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"message\":\"{{value}}\"}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::name] = "";
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::platform] = "notify";
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
    }
}
#endif // EXTENSION_HOMEASSISTANT

void MessageExtension::begin()
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
        setFont(FontSmall->name);
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
        if (text && width < -offsetX)
        {
            text.reset();
            messages.erase(messages.begin());
        }
        else if (text)
        {
            Display.clearFrame();
            text->draw(offsetX, offsetY);
            --offsetX;
            lastMillis = millis();
            Display.flush();
        }
        else if (messages.size())
        {
            text = std::make_unique<TextHandler>(messages.front(), font);
            offsetX = GRID_COLUMNS;
            offsetY = (GRID_ROWS - text->getHeight()) / 2;
            width = text->getWidth();
            if (!active)
            {
                Modes.setActive(false);
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
            Modes.setActive(true);
            active = false;
        }
    }
}

void MessageExtension::addMessage(std::string message)
{
    for (uint8_t i = 0; i <= repeat; ++i)
    {
        messages.push_back(message);
    }
    ESP_LOGD(name, "received");
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
        ESP_LOGD(name, "unknown font %s", fontName);
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

void MessageExtension::onReceive(const JsonDocument doc, const char *const source)
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
    if (doc["message"].is<std::string>())
    {
        addMessage(doc["message"].as<std::string>());
    }
}

#endif // EXTENSION_MESSAGE
