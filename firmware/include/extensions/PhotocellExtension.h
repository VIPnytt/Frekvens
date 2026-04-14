#pragma once

#if EXTENSION_PHOTOCELL

#include "modules/ExtensionModule.h"

class PhotocellExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"Photocell"};

    bool active = false;
    bool direction = false;
    bool pending = false;

    float gamma = 1.0F;

    uint8_t brightness = UINT8_MAX;

    int16_t counter = 0;

    uint16_t raw = 0;

    unsigned long lastMillis = 0;
    unsigned long _lastMillis = 0;

    void setGamma(float _gamma);

    void transmit();

public:
    explicit PhotocellExtension() : ExtensionModule(name) {};

    void configure() override;
    void begin() override;
    void handle() override;

    [[nodiscard]] bool getActive() const;
    void setActive(bool active);

    void onReceive(JsonObjectConst payload, std::string_view source) override;
    void onTransmit(JsonObjectConst payload, std::string_view source) override;
};

#endif // EXTENSION_PHOTOCELL
