#if EXTENSION_MESSAGE

#include "extensions/MessageExtension.h"

#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)
#include "services/FontsService.h"      // NOLINT(misc-include-cleaner)
#include "services/ModesService.h"

#include <Preferences.h>

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

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void MessageExtension::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_active")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"active":{{value}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:remote-tv");
        component[HomeAssistantAbbreviations::name].set(name);
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::payload_off].set("false");
        component[HomeAssistantAbbreviations::payload_on].set("true");
        component[HomeAssistantAbbreviations::platform].set("switch");
        component[HomeAssistantAbbreviations::state_off].set("False");
        component[HomeAssistantAbbreviations::state_on].set("True");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.active}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // EXTENSION_MESSAGE
