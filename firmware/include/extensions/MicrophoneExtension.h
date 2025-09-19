#pragma once

#include "config/constants.h"

#if EXTENSION_MICROPHONE

#include "modules/ExtensionModule.h"

class MicrophoneExtension : public ExtensionModule
{
private:
    bool
        active = false,
        detected = false,
        pending = false;

    uint16_t
        levelMax = UINT8_MAX,
        mic,
        threshold = INT8_MAX;

    unsigned long
        lastMillis = 0,
        _lastMillis = 0;

    void transmit();

public:
    MicrophoneExtension();

    void setup() override;
    void ready() override;
    void handle() override;

    bool get();
    void set(bool enable);
    void set(uint16_t floor);
    bool play();

    void receiverHook(const JsonDocument doc) override;
};

extern MicrophoneExtension *Microphone;

#endif // EXTENSION_MICROPHONE
