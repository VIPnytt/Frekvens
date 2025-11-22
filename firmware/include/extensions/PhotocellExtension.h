#pragma once

#include "config/constants.h"

#if EXTENSION_PHOTOCELL

#include "modules/ExtensionModule.h"

class PhotocellExtension : public ExtensionModule
{
private:
    bool
        active = false,
        direction = false,
        pending = false;

    float gamma = 1.0f;

    uint8_t brightness = UINT8_MAX;

    int16_t counter = 0;

    uint16_t raw = 0;

    unsigned long
        lastMillis = 0,
        _lastMillis = 0;

    void setGamma(float _gamma);

    void transmit();

public:
    PhotocellExtension();

    void setup() override;
    void ready() override;
    void handle() override;

    bool getActive() const;
    void setActive(bool active, const char *const source);

    void receiverHook(const JsonDocument doc, const char *const source) override;
    void transmitterHook(const JsonDocument &doc, const char *const source) override;
};

extern PhotocellExtension *Photocell;

#endif // EXTENSION_PHOTOCELL
