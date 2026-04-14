#if EXTENSION_MESSAGE

#include "extensions/MessageExtension.h"

#include "extensions/HomeAssistantExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"
#include "services/FontsService.h" // NOLINT(misc-include-cleaner)
#include "services/ModesService.h"

#include <Preferences.h>

#if EXTENSION_HOMEASSISTANT
void MessageExtension::configure()
{
    const std::string topic{std::string("frekvens/" HOSTNAME "/").append(name)};
    const HomeAssistantExtension &_ha = Extensions.HomeAssistant();
    {
        const std::string id{std::string(name).append("_font")};
        JsonObject component{(*_ha.discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
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
        component[HomeAssistantAbbreviations::unique_id].set(_ha.uniquePrefix + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.font}}");
    }
    {
        const std::string id{std::string(name).append("_notify")};
        JsonObject component{(*_ha.discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"message":"{{value}}"})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::name].set("");
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::platform].set("notify");
        component[HomeAssistantAbbreviations::unique_id].set(_ha.uniquePrefix + id);
    }
    {
        const std::string id{std::string(name).append("_repeat")};
        JsonObject component{(*_ha.discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
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
        component[HomeAssistantAbbreviations::unique_id].set(_ha.uniquePrefix + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.repeat}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

void MessageExtension::begin()
{
    Preferences Storage;
    Storage.begin(name.data(), true);
    if (Storage.isKey("font"))
    {
        fontName = Storage.getString("font").c_str();
    }
    if (Storage.isKey("repeat"))
    {
        repeat = Storage.getUShort("repeat");
    }
    Storage.end();
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
        else if (!messages.empty())
        {
            if (!font)
            {
                std::unique_ptr<const FontModule> _font = Fonts.get(fontName);
                font = std::move(_font);
            }
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
            font.reset();
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

void MessageExtension::setFont(std::string_view _fontName)
{
    if (const std::unique_ptr<const FontModule> _font = Fonts.get(_fontName))
    {
        fontName = _font->name.data();
        Preferences Storage;
        Storage.begin(name.data());
        Storage.putString("font", fontName.c_str());
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
        Storage.begin(name.data());
        Storage.putUShort("repeat", repeat);
        Storage.end();
        pending = true;
    }
}

void MessageExtension::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["font"].set(fontName);
    doc["repeat"].set(repeat);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void MessageExtension::onReceive(JsonObjectConst payload,
                                 std::string_view source) // NOLINT(misc-unused-parameters)
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
