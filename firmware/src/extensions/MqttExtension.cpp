#if EXTENSION_MQTT

#include "extensions/MqttExtension.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"

#include <WiFi.h>
#include <array>

MqttExtension *Mqtt = nullptr; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

MqttExtension::MqttExtension() : ExtensionModule("MQTT") { Mqtt = this; }

void MqttExtension::configure()
{
    client.onConnect(&onConnect);
    client.onMessage(&onMessage);
    client.onDisconnect(&onDisconnect);
    client.setCleanSession(false);
    client.setClientId(HOSTNAME);
    client.setWill("frekvens/" HOSTNAME "/availability", 1, true, std::array<uint8_t, 1>{0}.data(), 0);
#ifdef MQTT_PORT
    client.setServer(MQTT_HOST, MQTT_PORT);
#else
    client.setServer(MQTT_HOST, 1883);
#endif // MQTT_PORT
    client.setCredentials(MQTT_USER, MQTT_KEY);
    if (WiFi.isConnected())
    {
        client.connect();
    }
}

void MqttExtension::handle()
{
    if (!client.connected() && WiFi.isConnected() && millis() - lastMillis > UINT16_MAX)
    {
        lastMillis = millis();
        if (!client.connect() && client.queueSize() > INT8_MAX)
        {
            client.clearQueue();
            ESP_LOGD(Mqtt->name, "queue dropped"); // NOLINT(cppcoreguidelines-avoid-do-while)
        }
    }
}

void MqttExtension::disconnect()
{
    lastMillis = millis();
    if (client.connected())
    {
        client.publish("frekvens/" HOSTNAME "/availability", 1, true, std::array<uint8_t, 1>{0}.data(), 0);
        client.loop();
        client.disconnect();
    }
}

void MqttExtension::onConnect(bool sessionPresent)
{
    ESP_LOGD(Mqtt->name, "connected"); // NOLINT(cppcoreguidelines-avoid-do-while)
    if (!sessionPresent ||
        (!subscribed && esp_sleep_get_wakeup_cause() == esp_sleep_source_t::ESP_SLEEP_WAKEUP_UNDEFINED))
    {
        Mqtt->client.subscribe("frekvens/" HOSTNAME "/+/set", 2);
        subscribed = true;
    }
    Mqtt->client.publish("frekvens/" HOSTNAME "/availability", 1, true, "online");
}

void MqttExtension::onMessage(const espMqttClientTypes::MessageProperties &properties, // NOLINT(misc-unused-parameters)
                              const char *topic, const uint8_t *payload, size_t len,
                              size_t index, // NOLINT(misc-unused-parameters)
                              size_t total) // NOLINT(misc-unused-parameters)
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    if (deserializeJson(doc, payload, len))
    {
        return;
    }
    Device.receive(doc.as<JsonObjectConst>(),
                   Mqtt->name,
                   std::string(topic).substr(prefixLength, strlen(topic) - prefixLength - suffixLength).c_str());
}

void MqttExtension::onDisconnect(espMqttClientTypes::DisconnectReason reason)
{
    ESP_LOGD(Mqtt->name, "disconnected"); // NOLINT(cppcoreguidelines-avoid-do-while)
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-do-while)
    ESP_LOGV(Mqtt->name, "%s", espMqttClientTypes::disconnectReasonToString(reason));
}

void MqttExtension::onTransmit(JsonObjectConst payload, const char *source)
{
    const size_t length = measureJson(payload);
    std::vector<char> message(length + 1);
    serializeJson(payload, message.data(), length + 1);
    client.publish(std::string("frekvens/" HOSTNAME "/").append(source).c_str(),
                   payload["event"].isUnbound() ? 0 : 2,
                   false,
                   reinterpret_cast<const uint8_t *>(message.data()),
                   length);
}

#endif // EXTENSION_MQTT
