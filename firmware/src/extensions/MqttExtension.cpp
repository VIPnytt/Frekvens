#if EXTENSION_MQTT

#include "extensions/MqttExtension.h"

#include "config/constants.h"
#include "extensions/HomeAssistantExtension.h"
#include "services/ConnectivityService.h"
#include "services/DeviceService.h"

#include <WiFi.h>

MqttExtension *Mqtt = nullptr;

MqttExtension::MqttExtension() : ExtensionModule("MQTT")
{
    Mqtt = this;
}

void MqttExtension::configure()
{
    client.onConnect(&onConnect);
    client.onMessage(&onMessage);
    client.onDisconnect(&onDisconnect);
    client.setCleanSession(false);
    client.setClientId(HOSTNAME);
    client.setWill("frekvens/" HOSTNAME "/availability", 1, true, (const uint8_t[]){0}, 0);
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
            ESP_LOGD(Mqtt->name, "queue dropped");
        }
    }
}

void MqttExtension::disconnect()
{
    lastMillis = millis();
    if (client.connected())
    {
        client.publish("frekvens/" HOSTNAME "/availability", 1, true, (const uint8_t[]){0}, 0);
        client.loop();
        client.disconnect();
    }
}

void MqttExtension::onConnect(bool sessionPresent)
{
    ESP_LOGD(Mqtt->name, "connected");
    if (!sessionPresent || (!subscribed && esp_sleep_get_wakeup_cause() == esp_sleep_source_t::ESP_SLEEP_WAKEUP_UNDEFINED))
    {
        Mqtt->client.subscribe("frekvens/" HOSTNAME "/+/set", 2);
        subscribed = true;
    }
    Mqtt->client.publish("frekvens/" HOSTNAME "/availability", 1, true, "online");
}

void MqttExtension::onMessage(const espMqttClientTypes::MessageProperties &properties, const char *topic, const uint8_t *payload, size_t len, size_t index, size_t total)
{
    JsonDocument doc;
    if (deserializeJson(doc, payload, len))
    {
        return;
    }
    Device.receive(doc, Mqtt->name, std::string(topic).substr(prefixLength, strlen(topic) - prefixLength - suffixLength).c_str());
}

void MqttExtension::onDisconnect(espMqttClientTypes::DisconnectReason reason)
{
    ESP_LOGD(Mqtt->name, "disconnected");
    ESP_LOGV(Mqtt->name, "%s", espMqttClientTypes::disconnectReasonToString(reason));
}

void MqttExtension::onTransmit(const JsonDocument &doc, const char *const source)
{
    const size_t length = measureJson(doc);
    std::vector<char> payload(length + 1);
    serializeJson(doc, payload.data(), length + 1);
    client.publish(std::string("frekvens/" HOSTNAME "/").append(source).c_str(), doc["event"].isUnbound() ? 0 : 2, false, reinterpret_cast<const uint8_t *>(payload.data()), length);
}

#endif // EXTENSION_MQTT
