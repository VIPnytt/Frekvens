#pragma once

#include "config/constants.h"

#if EXTENSION_MQTT

#include <espMqttClient.h>

#include "modules/ExtensionModule.h"

class MqttExtension : public ExtensionModule
{
private:
    unsigned long lastMillis = 0;

    static inline bool subscribed = false;

    static constexpr size_t
        prefixLength = sizeof("frekvens/" HOSTNAME),
        suffixLength = sizeof("set");

    static void onConnect(bool sessionPresent);
    static void onDisconnect(espMqttClientTypes::DisconnectReason reason);
    static void onMessage(const espMqttClientTypes::MessageProperties &properties, const char *topic, const uint8_t *payload, size_t len, size_t index, size_t total);

public:
    MqttExtension();

    espMqttClient client;

    void setup() override;
    void handle() override;

    void disconnect();

    void transmitterHook(const JsonDocument &doc, const char *const source) override;
};

extern MqttExtension *Mqtt;

#endif // EXTENSION_MQTT
