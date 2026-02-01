#pragma once

#if EXTENSION_MICROPHONE

#include "modules/ExtensionModule.h"

class MicrophoneExtension : public ExtensionModule
{
private:
    bool active = false;
    bool detected = false;
    bool pending = false;

    uint16_t levelMax = UINT8_MAX;
    uint16_t mic = 0;
    uint16_t threshold = INT8_MAX;

    unsigned long lastMillis = 0;
    unsigned long _lastMillis = 0;

    void transmit();

public:
    MicrophoneExtension();

    void configure() override;
    void begin() override;
    void handle() override;

    bool getActive();
    void setActive(bool active);
    void setThreshold(uint16_t _threshold);
    bool isTriggered() const;

    void onReceive(const JsonDocument doc, const char *const source) override;
};

extern MicrophoneExtension *Microphone;

#endif // EXTENSION_MICROPHONE
