#pragma once

#if EXTENSION_MICROPHONE

#include "modules/ExtensionModule.h"

class MicrophoneExtension final : public ExtensionModule
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
    explicit MicrophoneExtension();

    void configure() override;
    void begin() override;
    void handle() override;

    [[nodiscard]] bool getActive() const;
    void setActive(bool active);
    void setThreshold(uint16_t _threshold);
    [[nodiscard]] bool isTriggered() const;

    void onReceive(JsonObjectConst payload, const char *source) override;
};

extern MicrophoneExtension *Microphone;

#endif // EXTENSION_MICROPHONE
