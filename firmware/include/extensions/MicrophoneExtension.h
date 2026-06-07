#pragma once

#if EXTENSION_MICROPHONE

#include "modules/ExtensionModule.h"

class MicrophoneExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"Microphone"};

    bool active{false};
    bool pending{false};
    bool triggered{true};

    uint8_t index{0U};
    uint8_t soundFloor{UINT8_MAX};

    uint16_t accumulated{0U};
    uint16_t mic{0U};
    uint16_t soundCeiling{0U};
    uint16_t threshold{INT8_MAX};

    unsigned long lastMillis{0UL};

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
