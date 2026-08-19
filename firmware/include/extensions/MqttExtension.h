#pragma once

#if EXTENSION_MQTT

#include "modules/ExtensionModule.h"

#include <array>
#include <espMqttClient.h>
#include <vector>

class MqttExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"MQTT"};

    unsigned long lastMillis{0UL};

    /**
 * Stores incoming MQTT message data.
 */
static inline std::vector<uint8_t> buffer{};

    static constexpr size_t prefixLength{sizeof("frekvens/" HOSTNAME "/") - 1U};
    static constexpr size_t suffixLength{sizeof("/set") - 1U};

    static void onConnect(bool sessionPresent);
    static void onDisconnect(espMqttClientTypes::DisconnectReason reason);
    static void onMessage(const espMqttClientTypes::MessageProperties &properties, const char *topic,
                          const uint8_t *payload, size_t len, size_t index, size_t total);

public:
    explicit MqttExtension() : ExtensionModule(name) {};

    static constexpr std::array<uint8_t, 1U> emptyMessage{0U};

    espMqttClient client{espMqttClientTypes::UseInternalTask::NO};

    void configure() override;
    void handle() override;

    void disconnect();

    void onTransmit(JsonObjectConst payload, std::string_view source) override;
};

#endif // EXTENSION_MQTT
