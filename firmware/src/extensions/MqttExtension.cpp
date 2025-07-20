#include "config/constants.h"

#if EXTENSION_MQTT

#include <WiFi.h>

#include "extensions/BuildExtension.h"
#include "extensions/HomeAssistantExtension.h"
#include "extensions/MqttExtension.h"
#include "services/DeviceService.h"
#include "services/NetworkService.h"

MqttExtension *Mqtt = nullptr;

MqttExtension::MqttExtension() : ExtensionModule("MQTT")
{
    Mqtt = this;
}

void MqttExtension::setup()
{
    client.onConnect(&onConnect);
    client.onMessage(&onMessage);
    client.onDisconnect(&onDisconnect);
    client.setCleanSession(false);
    client.setClientId(HOSTNAME);
    client.setWill("frekvens/" HOSTNAME "/availability", 1, true, reinterpret_cast<const uint8_t *>(""), 0);
#if defined(MQTT_PORT)
    client.setServer(MQTT_HOST, MQTT_PORT);
#else
    client.setServer(MQTT_HOST, 1883);
#endif
#if defined(MQTT_USER) && defined(MQTT_KEY)
    client.setCredentials(MQTT_USER, MQTT_KEY);
#endif
    if (WiFi.isConnected())
    {
        client.connect();
    }
#if EXTENSION_BUILD
    (*Build->config)[Config::h][__STRING(MQTT_HOST)] = MQTT_HOST;
#ifdef MQTT_PORT
    (*Build->config)[Config::h][__STRING(MQTT_PORT)] = MQTT_PORT;
#endif // MQTT_PORT
#if defined(MQTT_USER) && defined(MQTT_KEY)
    (*Build->config)[Config::h][__STRING(MQTT_USER)] = MQTT_USER;
    (*Build->config)[Config::h][__STRING(MQTT_KEY)] = "REDACTED";
#endif // defined(MQTT_USER) && defined(MQTT_KEY)
#endif // EXTENSION_BUILD
}

void MqttExtension::handle()
{
    if (pending)
    {
        pending = false;
        transmit();
    }
    else if (!client.connected() && WiFi.isConnected() && millis() - lastMillis > UINT16_MAX)
    {
        lastMillis = millis();
        if (!client.connect() && client.queueSize() > INT8_MAX)
        {
            client.clearQueue();
#ifdef F_DEBUG
            Serial.print(Mqtt->name);
            Serial.println(": queue dropped");
#endif // F_DEBUG
        }
    }
}

void MqttExtension::disconnect()
{
    lastMillis = millis();
    if (client.connected())
    {
        client.publish("frekvens/" HOSTNAME "/availability", 1, true, "offline");
        client.loop();
        client.disconnect();
    }
}

void MqttExtension::onConnect(bool sessionPresent)
{
#ifdef F_DEBUG
    Serial.print(Mqtt->name);
    Serial.println(": connected");
#endif
    if (!sessionPresent)
    {
        for (const char *const _name : Device.getNames())
        {
            Mqtt->client.subscribe(std::string("frekvens/" HOSTNAME "/").append(_name).append("/set").c_str(), 2);
        }
    }
    Mqtt->client.publish("frekvens/" HOSTNAME "/availability", 1, true, "online");
    Mqtt->pending = true;
}

void MqttExtension::onMessage(const espMqttClientTypes::MessageProperties &properties, const char *topic, const uint8_t *payload, size_t len, size_t index, size_t total)
{
#ifdef F_DEBUG
    if (len < total)
    {
        Serial.print(Mqtt->name);
        Serial.println(": chunked messages is currently not supported");
        return;
    }
#endif
    JsonDocument doc;
    if (deserializeJson(doc, payload, len))
    {
        return;
    }
    Device.receive(doc, Mqtt->name, std::string(topic).substr(prefixLength, strlen(topic) - prefixLength - suffixLength).c_str());
}

void MqttExtension::onDisconnect(espMqttClientTypes::DisconnectReason reason)
{
#ifdef F_VERBOSE
    Serial.print(Mqtt->name);
    Serial.print(": disconnected, ");
    Serial.println(espMqttClientTypes::disconnectReasonToString(reason));
#elif defined(F_DEBUG)
    Serial.print(Mqtt->name);
    Serial.println(": disconnected");
#endif // F_DEBUG
    Mqtt->pending = true;
}

void MqttExtension::transmit()
{
    JsonDocument doc;
    doc["host"] = client.connected() ? MQTT_HOST : nullptr;
    Device.transmit(doc, name);
}

void MqttExtension::transmitterHook(const JsonDocument &doc, const char *const source)
{
    const size_t length = measureJson(doc);
    uint8_t *payload = new uint8_t[length + 1];
    serializeJson(doc, reinterpret_cast<char *>(payload), length + 1);
    client.publish(std::string("frekvens/" HOSTNAME "/").append(source).c_str(), doc["event"].isUnbound() ? 0 : 2, false, payload, length);
    delete[] payload;
}

#endif // EXTENSION_MQTT
