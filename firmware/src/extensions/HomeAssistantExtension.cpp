#if EXTENSION_HOMEASSISTANT

#include "extensions/HomeAssistantExtension.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "extensions/MqttExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)
#include "services/ModesService.h"   // NOLINT(misc-include-cleaner)

#include <WiFi.h>
#include <array>
#include <regex>

HomeAssistantExtension *HomeAssistant = nullptr; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

HomeAssistantExtension::HomeAssistantExtension() : ExtensionModule("Home Assistant") { HomeAssistant = this; }

void HomeAssistantExtension::configure()
{
    const std::string topic{std::string("frekvens/" HOSTNAME "/").append(name)};
    {
        const std::string id{std::regex_replace(name, std::regex(R"(\s+)"), "").append("_main")};
        const std::string topicDisplay{std::string("frekvens/" HOSTNAME "/").append(Display.name)};
        JsonObject component{(*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::brightness_command_template].set(R"({"brightness":{{value}}})");
        component[HomeAssistantAbbreviations::brightness_command_topic].set(topicDisplay + "/set");
        component[HomeAssistantAbbreviations::brightness_state_topic].set(topicDisplay);
        component[HomeAssistantAbbreviations::brightness_value_template].set("{{value_json.brightness}}");
        component[HomeAssistantAbbreviations::command_topic].set(topicDisplay + "/set");
        component[HomeAssistantAbbreviations::effect_command_template].set(R"({"mode":"{{value}}"})");
        component[HomeAssistantAbbreviations::effect_command_topic].set(
            std::string("frekvens/" HOSTNAME "/").append(Modes.name).append("/set"));
        JsonArray effectList{component[HomeAssistantAbbreviations::effect_list].to<JsonArray>()};
        for (const ModeModule *mode : Modes.getAll())
        {
            effectList.add(mode->name);
        }
        component[HomeAssistantAbbreviations::effect_state_topic].set(
            std::string("frekvens/" HOSTNAME "/").append(Modes.name));
        component[HomeAssistantAbbreviations::effect_value_template].set("{{value_json.mode}}");
        component[HomeAssistantAbbreviations::icon].set("mdi:dots-grid");
        component[HomeAssistantAbbreviations::name].set("");
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::on_command_type].set("brightness");
        component[HomeAssistantAbbreviations::payload_off].set(payloadOff);
        component[HomeAssistantAbbreviations::payload_on].set(payloadOn);
        component[HomeAssistantAbbreviations::platform].set("light");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::state_value_template].set(
            std::string("{{value_json.").append(Display.name).append(".power}}"));
        component[HomeAssistantAbbreviations::unique_id].set(HomeAssistant->uniquePrefix + id);
    }
}

void HomeAssistantExtension::begin()
{
    {
        JsonObject availability{(*discovery)[HomeAssistantAbbreviations::availability].to<JsonObject>()};
        availability[HomeAssistantAbbreviations::payload_not_available].set("");
        availability[HomeAssistantAbbreviations::topic].set("frekvens/" HOSTNAME "/availability");
    }
    {
        JsonObject device{(*discovery)[HomeAssistantAbbreviations::device].to<JsonObject>()};
#if EXTENSION_WEBAPP
        device[HomeAssistantAbbreviations::configuration_url].set("http://" HOSTNAME ".local");
#endif // EXTENSION_WEBAPP
        {
            JsonArray _connections{device[HomeAssistantAbbreviations::connections].to<JsonArray>()};
            {
                JsonArray _wifi{_connections.add<JsonArray>()};
                _wifi.add("mac");
                _wifi.add(WiFi.macAddress());
            }
        }
        device[HomeAssistantAbbreviations::hw_version].set(ARDUINO_BOARD);
        device[HomeAssistantAbbreviations::identifiers].to<JsonArray>().add(std::format("0x{:x}", ESP.getEfuseMac()));
        device[HomeAssistantAbbreviations::manufacturer].set(MANUFACTURER);
        device[HomeAssistantAbbreviations::model].set(MODEL);
        device[HomeAssistantAbbreviations::name].set(NAME);
        device[HomeAssistantAbbreviations::sw_version].set("Frekvens " VERSION);
        {
            JsonObject origin{(*discovery)[HomeAssistantAbbreviations::origin].to<JsonObject>()};
            origin[HomeAssistantAbbreviations::name].set("Frekvens");
            origin[HomeAssistantAbbreviations::support_url].set(
                "https://github.com/VIPnytt/Frekvens/blob/main/docs/SUPPORT.md");
            origin[HomeAssistantAbbreviations::sw_version].set(VERSION);
        }
    }
    const size_t length{measureJson(*discovery)};
    std::vector<uint8_t> payload(length + 1);
    serializeJson(*discovery, payload.data(), length + 1);
    delete discovery;
    discovery = nullptr;
    Mqtt->client.publish(discoveryTopic.c_str(), 0, true, payload.data(), length);
}

void HomeAssistantExtension::handle()
{
    if (pending)
    {
        pending = false;
        transmit();
    }
}

void HomeAssistantExtension::undiscover()
{
    Mqtt->client.publish(discoveryTopic.c_str(), 1, true, std::array<uint8_t, 1>{0}.data(), 0);
    ESP_LOGW(name, "discovery packet removed"); // NOLINT(cppcoreguidelines-avoid-do-while)
}

void HomeAssistantExtension::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc[Display.name]["power"].set(Display.getPower() ? payloadOn : payloadOff);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void HomeAssistantExtension::onTransmit(JsonObjectConst payload, const char *source)
{
    // Display: Power
    if (!strcmp(source, Display.name) && payload["power"].is<bool>())
    {
        pending = true;
    }
    // Remove
    else if (payload["action"].is<const char *>() && !strcmp(payload["action"].as<const char *>(), "remove"))
    {
        undiscover();
    }
}

#endif // EXTENSION_HOMEASSISTANT
