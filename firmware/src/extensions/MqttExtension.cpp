#if EXTENSION_MQTT

#include "extensions/MqttExtension.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/ConnectivityService.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"
#include "services/ModesService.h"

#include <WiFi.h>
#include <array>

void MqttExtension::configure()
{
    client.onConnect(&onConnect);
    client.onMessage(&onMessage);
    client.onDisconnect(&onDisconnect);
    client.setCleanSession(false);
    client.setClientId(HOSTNAME);
    client.setWill("frekvens/" HOSTNAME "/availability",
                   static_cast<uint8_t>(espMqttClientTypes::SubscribeReturncode::QOS1),
                   true,
                   emptyMessage.data(),
                   emptyMessage.size() - 1U);
#ifdef MQTT_PORT
    client.setServer(MQTT_HOST, MQTT_PORT);
#else
    client.setServer(MQTT_HOST, 1883U);
#endif // MQTT_PORT
    client.setCredentials(MQTT_USER, MQTT_KEY);
    if (WiFi.isConnected())
    {
        client.connect();
    }
}

/**
 * @brief Publishes the device's Home Assistant MQTT discovery configuration.
 */
void MqttExtension::begin()
{
    const std::string topic{std::string("frekvens/" HOSTNAME "/")};
    const std::string unique{std::format("0x{:x}_", ESP.getEfuseMac())};
    JsonDocument discovery;
    for (ServiceModule *service : std::array<ServiceModule *, 4U>{
             &Connectivity,
             &Device,
             &Display,
             &Modes,
         })
    {
        service->onHomeAssistant(discovery, topic, unique);
    }
    for (ExtensionModule *extension : Extensions.getAll())
    {
        extension->onHomeAssistant(discovery, topic, unique);
    }
    for (const std::string_view _mode : Modes.names)
    {
        Modes.getMode(_mode)->onHomeAssistant(discovery, topic, unique);
    }
    {
        JsonObject availability{discovery[HomeAssistantAbbreviations::availability].to<JsonObject>()};
        availability[HomeAssistantAbbreviations::payload_not_available].set("");
        availability[HomeAssistantAbbreviations::topic].set("frekvens/" HOSTNAME "/availability");
    }
    {
        JsonObject device{discovery[HomeAssistantAbbreviations::device].to<JsonObject>()};
#if EXTENSION_WEBAPP
        device[HomeAssistantDeviceAbbreviations::configuration_url].set("http://" HOSTNAME ".local");
#endif // EXTENSION_WEBAPP
        {
            device[HomeAssistantDeviceAbbreviations::connections][0U][0U].set("mac");
            device[HomeAssistantDeviceAbbreviations::connections][0U][1U].set(WiFi.macAddress());
        }
        device[HomeAssistantDeviceAbbreviations::hw_version].set(ARDUINO_BOARD);
        device[HomeAssistantDeviceAbbreviations::identifiers][0U].set(std::format("0x{:x}", ESP.getEfuseMac()));
        device[HomeAssistantDeviceAbbreviations::manufacturer].set(MANUFACTURER);
        device[HomeAssistantDeviceAbbreviations::model].set(MODEL);
        device[HomeAssistantDeviceAbbreviations::name].set(NAME);
        device[HomeAssistantDeviceAbbreviations::sw_version].set("Frekvens " VERSION);
        {
            JsonObject origin{discovery[HomeAssistantAbbreviations::origin].to<JsonObject>()};
            origin[HomeAssistantOriginAbbreviations::name].set("Frekvens");
            origin[HomeAssistantOriginAbbreviations::support_url].set(
                "https://github.com/VIPnytt/Frekvens/blob/main/docs/SUPPORT.md");
            origin[HomeAssistantOriginAbbreviations::sw_version].set(VERSION);
        }
    }
    const size_t length{measureJson(discovery)};
    std::vector<uint8_t> payload(length + 1U);
    serializeJson(discovery, payload.data(), length + 1U);
    client.publish(discoveryTopic.c_str(),
                   static_cast<uint8_t>(espMqttClientTypes::SubscribeReturncode::QOS0),
                   true,
                   payload.data(),
                   length);
}

void MqttExtension::handle()
{
    client.loop();
    if (pending)
    {
        pending = false;
        transmit();
    }
}

/**
 * @brief Disconnects from the MQTT broker after publishing a retained unavailable status.
 */
void MqttExtension::disconnect()
{
    lastMillis = millis();
    if (client.connected())
    {
        client.publish("frekvens/" HOSTNAME "/availability",
                       static_cast<uint8_t>(espMqttClientTypes::SubscribeReturncode::QOS1),
                       true,
                       emptyMessage.data(),
                       emptyMessage.size() - 1U);
        client.loop();
        client.disconnect();
    }
}

/**
 * @brief Transmits the display power state to Home Assistant.
 */
void MqttExtension::transmit()
{
    JsonDocument doc{};
    doc[Display.name]["power"].set(Display.getPower() ? payloadOn : payloadOff);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

/**
 * @brief Handles a successful MQTT connection.
 *
 * Subscribes to device set commands and publishes the retained online availability status.
 *
 * @param sessionPresent Indicates whether the broker restored a previous session.
 */
void MqttExtension::onConnect(bool sessionPresent)
{
    ESP_LOGD(name.data(), "connected"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    client.subscribe("frekvens/" HOSTNAME "/+/set",
                     static_cast<uint8_t>(espMqttClientTypes::SubscribeReturncode::QOS2));
    client.publish("frekvens/" HOSTNAME "/availability",
                   static_cast<uint8_t>(espMqttClientTypes::SubscribeReturncode::QOS1),
                   true,
                   "online");
}

/**
 * @brief Adds the Home Assistant light component configuration to a discovery document.
 *
 * @param discovery Discovery document to update.
 * @param topic Base MQTT topic for the component state.
 * @param unique Prefix used to construct the component's unique identifier.
 */
void MqttExtension::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{"HomeAssistant_main"};
        const std::string topicDisplay{std::string("frekvens/" HOSTNAME "/").append(Display.name)};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::brightness_command_template].set(R"({"brightness":{{value}}})");
        component[HomeAssistantAbbreviations::brightness_command_topic].set(topicDisplay + "/set");
        component[HomeAssistantAbbreviations::brightness_state_topic].set(topicDisplay);
        component[HomeAssistantAbbreviations::brightness_value_template].set("{{value_json.brightness}}");
        component[HomeAssistantAbbreviations::command_topic].set(topicDisplay + "/set");
        component[HomeAssistantAbbreviations::effect_command_template].set(R"({"mode":"{{value}}"})");
        component[HomeAssistantAbbreviations::effect_command_topic].set(
            std::string("frekvens/" HOSTNAME "/").append(Modes.name).append("/set"));
        JsonArray effectList{component[HomeAssistantAbbreviations::effect_list].to<JsonArray>()};
        for (const std::string_view _mode : Modes.names)
        {
            effectList.add(_mode);
        }
        component[HomeAssistantAbbreviations::effect_state_topic].set(
            std::string("frekvens/" HOSTNAME "/").append(Modes.name));
        component[HomeAssistantAbbreviations::effect_value_template].set("{{value_json.mode}}");
        component[HomeAssistantAbbreviations::icon].set("mdi:dots-grid");
        component[HomeAssistantAbbreviations::name].set("");
        component[HomeAssistantAbbreviations::on_command_type].set("brightness");
        component[HomeAssistantAbbreviations::payload_off].set(payloadOff);
        component[HomeAssistantAbbreviations::payload_on].set(payloadOn);
        component[HomeAssistantAbbreviations::platform].set("light");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::state_value_template].set(
            std::string("{{value_json.").append(Display.name).append(".power}}"));
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
    }
}

/**
 * @brief Processes an MQTT message and forwards valid JSON payloads to the device.
 *
 * Reassembles fragmented payloads before deserializing them. Invalid JSON payloads
 * are ignored.
 *
 * @param properties MQTT message properties.
 * @param topic Full MQTT topic associated with the message.
 * @param payload Message payload or payload fragment.
 * @param len Length of the payload or fragment.
 * @param index Offset of the fragment within the complete payload.
 * @param total Total length of the complete payload.
 */
void MqttExtension::onMessage(const espMqttClientTypes::MessageProperties &properties, const char *topic,
                              const uint8_t *payload, size_t len, size_t index, size_t total)
{
    if (index != 0U || len != total)
    {
        if (index == 0U)
        {
            buffer.resize(total);
        }
        std::copy_n(payload, len, buffer.begin() + static_cast<std::ptrdiff_t>(index));
        if (index + len != total)
        {
            return;
        }
        payload = buffer.data();
        len = buffer.size();
    }
    JsonDocument doc{};
    if (deserializeJson(doc, payload, len) == DeserializationError::Code::Ok)
    {
        const std::string_view _topic{topic};
        Device.receive(
            doc.as<JsonObjectConst>(), name, _topic.substr(prefixLength, _topic.size() - prefixLength - suffixLength));
    }
}

/**
 * @brief Logs the MQTT disconnection and its reason.
 *
 * @param reason Reason reported for the disconnection.
 */
void MqttExtension::onDisconnect(espMqttClientTypes::DisconnectReason reason)
{
    ESP_LOGD(name.data(), "disconnected"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    ESP_LOGV(name.data(), "%s", espMqttClientTypes::disconnectReasonToString(reason));
}

/**
 * @brief Publishes a device event payload to its MQTT topic.
 *
 * @param payload JSON payload to publish.
 * @param source Device source used to construct the MQTT topic.
 */
void MqttExtension::onTransmit(JsonObjectConst payload, std::string_view source)
{
    const size_t length{measureJson(payload)};
    std::vector<char> message(length + 1U);
    serializeJson(payload, message.data(), length + 1U);
    client.publish(std::string("frekvens/" HOSTNAME "/").append(source).c_str(),
                   payload["event"].isUnbound() ? static_cast<uint8_t>(espMqttClientTypes::SubscribeReturncode::QOS0)
                                                : static_cast<uint8_t>(espMqttClientTypes::SubscribeReturncode::QOS2),
                   false,
                   reinterpret_cast<const uint8_t *>(message.data()),
                   length);
    // Display: Power
    if (source == Display.name && payload["power"].is<bool>())
    {
        pending = true;
    }
}

#endif // EXTENSION_MQTT
