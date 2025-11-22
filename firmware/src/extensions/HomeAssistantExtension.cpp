#include "config/constants.h"

#if EXTENSION_HOMEASSISTANT

#include <regex>
#include <WiFi.h>

#include "extensions/HomeAssistantExtension.h"
#include "extensions/MqttExtension.h"
#include "services/ConnectivityService.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ModesService.h"
#include "services/WebServerService.h"

HomeAssistantExtension *HomeAssistant = nullptr;

HomeAssistantExtension::HomeAssistantExtension() : ExtensionModule("Home Assistant")
{
    HomeAssistant = this;
}

void HomeAssistantExtension::setup()
{
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string
            id = std::regex_replace(name, std::regex("\\s+"), "").append("_main"),
            topicDisplay = std::string("frekvens/" HOSTNAME "/").append(Display.name);
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::brightness_command_template] = "{\"brightness\":{{value-1}}}";
        component[HomeAssistantAbbreviations::brightness_command_topic] = topicDisplay + "/set";
        component[HomeAssistantAbbreviations::brightness_scale] = 1 << 8;
        component[HomeAssistantAbbreviations::brightness_state_topic] = topicDisplay;
        component[HomeAssistantAbbreviations::brightness_value_template] = "{{value_json.brightness+1}}";
        component[HomeAssistantAbbreviations::command_topic] = topicDisplay + "/set";
        component[HomeAssistantAbbreviations::effect_command_template] = "{\"mode\":\"{{value}}\"}";
        component[HomeAssistantAbbreviations::effect_command_topic] = std::string("frekvens/" HOSTNAME "/").append(Modes.name).append("/set");
        JsonArray effectList = component[HomeAssistantAbbreviations::effect_list].to<JsonArray>();
        for (const ModeModule *mode : Modes.getAll())
        {
            effectList.add(mode->name);
        }
        component[HomeAssistantAbbreviations::effect_state_topic] = std::string("frekvens/" HOSTNAME "/").append(Modes.name);
        component[HomeAssistantAbbreviations::effect_value_template] = "{{value_json.mode}}";
        component[HomeAssistantAbbreviations::icon] = "mdi:dots-grid";
        component[HomeAssistantAbbreviations::name] = "";
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::on_command_type] = "brightness";
        component[HomeAssistantAbbreviations::payload_off] = payloadOff;
        component[HomeAssistantAbbreviations::payload_on] = payloadOn;
        component[HomeAssistantAbbreviations::platform] = "light";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::state_value_template] = std::string("{{value_json.").append(Display.name).append(".power}}");
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
    }
}

void HomeAssistantExtension::ready()
{
    {
        JsonObject availability = (*discovery)[HomeAssistantAbbreviations::availability].to<JsonObject>();
        availability[HomeAssistantAbbreviations::payload_not_available] = "";
        availability[HomeAssistantAbbreviations::topic] = "frekvens/" HOSTNAME "/availability";
    }
    {
        JsonObject device = (*discovery)[HomeAssistantAbbreviations::device].to<JsonObject>();
#if EXTENSION_WEBAPP
        device[HomeAssistantAbbreviations::configuration_url] = "http://" HOSTNAME ".local";
#endif // EXTENSION_WEBAPP
        {
            JsonArray _connections = device[HomeAssistantAbbreviations::connections].to<JsonArray>();
            {
                JsonArray _wifi = _connections.add<JsonArray>();
                _wifi.add("mac");
                _wifi.add(WiFi.macAddress());
            }
        }
        device[HomeAssistantAbbreviations::hw_version] = BOARD__NAME;
        device[HomeAssistantAbbreviations::identifiers].to<JsonArray>().add(std::format("0x{:x}", ESP.getEfuseMac()));
        device[HomeAssistantAbbreviations::manufacturer] = MANUFACTURER;
        device[HomeAssistantAbbreviations::model] = MODEL;
        device[HomeAssistantAbbreviations::name] = NAME;
        device[HomeAssistantAbbreviations::sw_version] = "Frekvens " VERSION;
        {
            JsonObject origin = (*discovery)[HomeAssistantAbbreviations::origin].to<JsonObject>();
            origin[HomeAssistantAbbreviations::name] = "Frekvens";
            origin[HomeAssistantAbbreviations::support_url] = "https://github.com/VIPnytt/Frekvens/blob/main/docs/SUPPORT.md";
            origin[HomeAssistantAbbreviations::sw_version] = VERSION;
        }
    }
    const size_t length = measureJson(*discovery);
    uint8_t *payload = new uint8_t[length];
    serializeJson(*discovery, payload, length);
    delete discovery;
    discovery = nullptr;
    Mqtt->client.publish(discoveryTopic.c_str(), 0, true, payload, length);
    delete[] payload;
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
    Mqtt->client.publish(discoveryTopic.c_str(), 1, true, (const uint8_t[]){0}, 0);
    ESP_LOGW(name, "discovery packet removed");
}

void HomeAssistantExtension::transmit()
{
    JsonDocument doc;
    doc[Display.name]["power"] = Display.getPower() ? payloadOn : payloadOff;
    Device.transmit(doc, name);
}

void HomeAssistantExtension::transmitterHook(const JsonDocument &doc, const char *const source)
{
    // Display: Power
    if (!strcmp(source, Display.name) && doc["power"].is<bool>())
    {
        pending = true;
    }
    // Remove
    else if (doc["action"].is<const char *>() && !strcmp(doc["action"].as<const char *>(), "remove"))
    {
        undiscover();
    }
}

#endif // EXTENSION_HOMEASSISTANT
