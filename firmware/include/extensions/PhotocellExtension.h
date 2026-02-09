#pragma once

#if EXTENSION_PHOTOCELL

#include "modules/ExtensionModule.h"

class PhotocellExtension final : public ExtensionModule
{
private:
    bool active = false;
    bool direction = false;
    bool pending = false;

    float gamma = 1.0f;

    uint8_t brightness = UINT8_MAX;

    int16_t counter = 0;

    uint16_t raw = 0;

    unsigned long lastMillis = 0;
    unsigned long _lastMillis = 0;

    void setGamma(float _gamma);

    void transmit();

public:
    explicit PhotocellExtension();

    void configure() override;
    void begin() override;
    void handle() override;

    [[nodiscard]] bool getActive() const;
    void setActive(bool active);

    void onReceive(JsonObjectConst payload, const char *source) override;
    void onTransmit(JsonObjectConst payload, const char *source) override;
};

extern PhotocellExtension *Photocell;

#endif // EXTENSION_PHOTOCELL
