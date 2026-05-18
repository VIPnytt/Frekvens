#pragma once

#if EXTENSION_PHOTOCELL

#include "modules/ExtensionModule.h"

class PhotocellExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"Photocell"};

    bool active{false};
    bool direction{false};
    bool pending{false};

    float gamma{1.0F};

    int16_t debounce{0};

    uint8_t brightness{UINT8_MAX};

    uint16_t raw{0U};

    unsigned long lastMillis{0UL};
    unsigned long _lastMillis{0UL};

    void setGamma(float _gamma);

    void transmit();

public:
    explicit PhotocellExtension() : ExtensionModule(name) {};

    void configure() override;
    void begin() override;
    void handle() override;

    [[nodiscard]] bool getActive() const;
    void setActive(bool _active);

    void onReceive(JsonObjectConst payload, std::string_view source) override;
    void onTransmit(JsonObjectConst payload, std::string_view source) override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif
};

#endif // EXTENSION_PHOTOCELL
