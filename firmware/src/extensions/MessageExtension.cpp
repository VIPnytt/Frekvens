#if EXTENSION_MESSAGE

#include "extensions/MessageExtension.h"

#include "extensions/HomeAssistantExtension.h"
#include "fonts/SmallFont.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"
#include "services/ModesService.h"

#include <Preferences.h>

MessageExtension *Message = nullptr; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

MessageExtension::MessageExtension() : ExtensionModule("Message") { Message = this; }

#if EXTENSION_HOMEASSISTANT
void MessageExtension::configure()
{
    const std::string topic{std::string("frekvens/" HOSTNAME "/").append(name)};
    {
        const std::string id{std::string(name).append("_font")};
        JsonObject component{(*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
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
        component[HomeAssistantAbbreviations::unique_id].set(HomeAssistant->uniquePrefix + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.font}}");
    }
    {
        const std::string id{std::string(name).append("_notify")};
        JsonObject component{(*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"message":"{{value}}"})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::name].set("");
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::platform].set("notify");
        component[HomeAssistantAbbreviations::unique_id].set(HomeAssistant->uniquePrefix + id);
    }
    {
        const std::string id{std::string(name).append("_repeat")};
        JsonObject component{(*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template] = R"({"repeat":"{{value}}"})";
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:counter");
        component[HomeAssistantAbbreviations::max].set(UINT8_MAX);
        component[HomeAssistantAbbreviations::mode].set("box");
        component[HomeAssistantAbbreviations::name].set(std::string(name).append(" repeat"));
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::platform].set("number");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(HomeAssistant->uniquePrefix + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.repeat}}");
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
    }
    if (!font)
    {
        setFont(SmallFont::name);
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
            text = std::make_unique<TextHandler>(messages.front(), *font);
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
            JsonDocument doc; // NOLINT(misc-const-correctness)
            doc["event"].set(messages.front());
            Device.transmit(doc.as<JsonObjectConst>(), name, false);
        }
        else if (active)
        {
            Display.setFrame(frame);
            Modes.setActive(true);
            active = false;
        }
    }
}

void MessageExtension::addMessage(std::string message) // NOLINT(readability-make-member-function-const)
{
    for (uint8_t i = 0; i <= repeat; ++i)
    {
        messages.push_back(message);
    }
    ESP_LOGD(name, "received");
}

void MessageExtension::setFont(std::string_view fontName)
{
    if (std::unique_ptr<FontModule> _font = Fonts.get(fontName))
    {
        font = std::move(_font);
        Preferences Storage;
        Storage.begin(name);
        Storage.putString("font", font->name.data());
        Storage.end();
        pending = true;
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
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["font"].set(font->name);
    doc["repeat"].set(repeat);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void MessageExtension::onReceive(JsonObjectConst payload,
                                 const char *source) // NOLINT(misc-unused-parameters)
{
    // Font
    if (payload["font"].is<std::string_view>())
    {
        setFont(payload["font"].as<std::string_view>());
    }
    // Repeat
    if (payload["repeat"].is<uint8_t>())
    {
        setRepeat(payload["repeat"].as<uint8_t>());
    }
    //  Message
    if (payload["message"].is<std::string>())
    {
        addMessage(payload["message"].as<std::string>());
    }
}

#endif // EXTENSION_MESSAGE
