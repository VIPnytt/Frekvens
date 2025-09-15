#pragma once

#include "config/constants.h"

#if EXTENSION_PHOTOCELL

#include "modules/ExtensionModule.h"

class PhotocellExtension : public ExtensionModule
{
private:
    bool
        active = false,
        pending = false;

    float gamma = 1.0f;

    int8_t
        debounce = 0,
        threshold = 1 << 6;

    uint8_t brightness = UINT8_MAX;

    uint16_t
        level = 0,
        levelMax = 0,
        levelMin = (1 << 12) - 1;

    unsigned long
        lastMillis = 0,
        _lastMillis = 0;

    void read();
    void setBrightness(uint8_t _brightness);

    void transmit();

public:
    PhotocellExtension();

    void setup() override;
    void ready() override;
    void handle() override;

    bool get();
    void set(bool enable);
    void setGamma(float _gamma);
    void setThreshold(int8_t _threshold);
    void reset();

    void receiverHook(const JsonDocument doc) override;
    void transmitterHook(const JsonDocument &doc, const char *const source) override;
};

extern PhotocellExtension *Photocell;

#endif // EXTENSION_PHOTOCELL
