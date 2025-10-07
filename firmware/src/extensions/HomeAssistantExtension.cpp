#include "config/constants.h"

#if EXTENSION_HOMEASSISTANT

#include <regex>
#include <WiFi.h>

#include "extensions/BuildExtension.h"
#include "extensions/HomeAssistantExtension.h"
#include "extensions/MqttExtension.h"
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
#if EXTENSION_BUILD && defined(HOMEASSISTANT_TOPIC)
    (*Build->config)[Config::h][__STRING(HOMEASSISTANT_TOPIC)] = HOMEASSISTANT_TOPIC;
#endif // EXTENSION_BUILD && defined(HOMEASSISTANT_TOPIC)

    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string
            id = std::regex_replace(name, std::regex("\\s+"), "").append("_main"),
            topicDisplay = std::string("frekvens/" HOSTNAME "/").append(Display.name),
            topicModes = std::string("frekvens/" HOSTNAME "/").append(Modes.name);
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::brightness_command_template] = "{\"brightness\":{{value-1}}}";
        component[Abbreviations::brightness_command_topic] = topicDisplay + "/set";
        component[Abbreviations::brightness_scale] = 1U << 8;
        component[Abbreviations::brightness_state_topic] = topicDisplay;
        component[Abbreviations::brightness_value_template] = "{{value_json.brightness+1}}";
        component[Abbreviations::command_topic] = topicDisplay + "/set";
        component[Abbreviations::effect_command_template] = "{\"mode\":\"{{value}}\"}";
        component[Abbreviations::effect_command_topic] = topicModes + "/set";
        JsonArray effectList = component[Abbreviations::effect_list].to<JsonArray>();
        for (const ModeModule *mode : Modes.getAll())
        {
            effectList.add(mode->name);
        }
        component[Abbreviations::effect_state_topic] = std::string("frekvens/" HOSTNAME "/").append(Modes.name);
        component[Abbreviations::effect_value_template] = "{{value_json.mode}}";
        component[Abbreviations::icon] = "mdi:dots-grid";
        component[Abbreviations::name] = "";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::on_command_type] = "brightness";
        component[Abbreviations::payload_off] = payloadOff;
        component[Abbreviations::payload_on] = payloadOn;
        component[Abbreviations::platform] = "light";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::state_value_template] = std::string("{{value_json.").append(Display.name).append(".power}}");
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
    }
}

void HomeAssistantExtension::ready()
{
    {
        JsonObject availability = (*discovery)[Abbreviations::availability].to<JsonObject>();
        availability[Abbreviations::payload_not_available] = "";
        availability[Abbreviations::topic] = "frekvens/" HOSTNAME "/availability";
    }
    {
        JsonObject device = (*discovery)[Abbreviations::device].to<JsonObject>();
#if EXTENSION_WEBAPP
        device[Abbreviations::configuration_url] = std::string("http://").append(Connectivity.domain);
#endif // EXTENSION_WEBAPP
        {
            JsonArray _connections = device[Abbreviations::connections].to<JsonArray>();
            {
                JsonArray _wifi = _connections.add<JsonArray>();
                _wifi.add("mac");
                _wifi.add(WiFi.macAddress());
            }
            {
                JsonArray _wifi = _connections.add<JsonArray>();
                _wifi.add("mac");
                _wifi.add(WiFi.softAPmacAddress());
            }
        }
#ifdef BOARD_NAME
        device[Abbreviations::hw_version] = BOARD_NAME;
#endif // BOARD_NAME
        device[Abbreviations::identifiers].to<JsonArray>().add(String(ESP.getEfuseMac(), HEX));
        device[Abbreviations::manufacturer] = MANUFACTURER;
        device[Abbreviations::model] = MODEL;
        device[Abbreviations::name] = NAME;
        device[Abbreviations::sw_version] = "Frekvens " VERSION;
        {
            JsonObject origin = (*discovery)[Abbreviations::origin].to<JsonObject>();
            origin[Abbreviations::name] = "Frekvens";
            origin[Abbreviations::support_url] = Device.repository;
            origin[Abbreviations::sw_version] = VERSION;
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
    Mqtt->client.publish(discoveryTopic.c_str(), 1, true, reinterpret_cast<const uint8_t *>(""), 0);
#ifdef F_INFO
    Serial.printf("%s: discovery packet removed\n", name);
#endif
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
