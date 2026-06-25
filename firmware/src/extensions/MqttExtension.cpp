#if EXTENSION_MQTT

#include "extensions/MqttExtension.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/ExtensionsService.h"

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

void MqttExtension::handle()
{
    client.loop();
#if EXTENSION_STATUSLED
    if (!client.connected() && WiFi.isConnected() && millis() - lastMillis > UINT8_MAX)
    {
        lastMillis = millis();
        Extensions.StatusLed().warning();
    }
#endif // EXTENSION_STATUSLED
}

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

void MqttExtension::onConnect(bool sessionPresent) // NOLINT(misc-unused-parameters)
{
    ESP_LOGD("MQTT", "connected"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    Extensions.MQTT().client.subscribe("frekvens/" HOSTNAME "/+/set",
                                       static_cast<uint8_t>(espMqttClientTypes::SubscribeReturncode::QOS2));
    Extensions.MQTT().client.publish("frekvens/" HOSTNAME "/availability",
                                     static_cast<uint8_t>(espMqttClientTypes::SubscribeReturncode::QOS1),
                                     true,
                                     "online");
}

void MqttExtension::onMessage(const espMqttClientTypes::MessageProperties &properties, // NOLINT(misc-unused-parameters)
                              const char *topic, const uint8_t *payload, size_t len, size_t index, size_t total)
{
    if (index != 0 || len != total)
    {
        if (index == 0)
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
    JsonDocument doc; // NOLINT(misc-const-correctness)
    if (deserializeJson(doc, payload, len) == DeserializationError::Code::Ok)
    {
        const std::string_view _topic{topic};
        Device.receive(
            doc.as<JsonObjectConst>(), name, _topic.substr(prefixLength, _topic.size() - prefixLength - suffixLength));
    }
}

void MqttExtension::onDisconnect(espMqttClientTypes::DisconnectReason reason) // NOLINT(misc-unused-parameters)
{
    ESP_LOGD("MQTT", "disconnected"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    ESP_LOGV("MQTT", "%s", espMqttClientTypes::disconnectReasonToString(reason));
}

void MqttExtension::onTransmit(JsonObjectConst payload, std::string_view source)
{
    const size_t length = measureJson(payload);
    std::vector<char> message(length + 1);
    serializeJson(payload, message.data(), length + 1);
    client.publish(std::string("frekvens/" HOSTNAME "/").append(source).c_str(),
                   payload["event"].isUnbound() ? static_cast<uint8_t>(espMqttClientTypes::SubscribeReturncode::QOS0)
                                                : static_cast<uint8_t>(espMqttClientTypes::SubscribeReturncode::QOS2),
                   false,
                   reinterpret_cast<const uint8_t *>(message.data()),
                   length);
}

#endif // EXTENSION_MQTT
