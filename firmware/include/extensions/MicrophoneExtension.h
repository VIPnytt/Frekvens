#pragma once

#if EXTENSION_MICROPHONE

#include "modules/ExtensionModule.h"

class MicrophoneExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"Microphone"};

    static inline uint16_t levelMax = UINT8_MAX;

    bool active = false;
    bool detected = false;
    bool pending = false;

    uint16_t mic = 0;
    uint16_t threshold = INT8_MAX;

    unsigned long lastMillis = 0;
    unsigned long _lastMillis = 0;

    void transmit();

public:
    explicit MicrophoneExtension() : ExtensionModule(name) {};

    void configure() override;
    void begin() override;
    void handle() override;

    [[nodiscard]] bool getActive() const;
    void setActive(bool _active);
    void setThreshold(uint16_t _threshold);
    [[nodiscard]] bool isTriggered() const;

    void onReceive(JsonObjectConst payload, std::string_view source) override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif
};

#endif // EXTENSION_MICROPHONE
