#pragma once

#include "config/constants.h"

#if EXTENSION_MICROPHONE

#include "modules/ExtensionModule.h"

class MicrophoneExtension : public ExtensionModule
{
private:
#ifdef MIC_FLOOR
    const uint16_t floor = MIC_FLOOR;
#else
    const uint16_t floor = 1 << 10;
#endif // MIC_FLOOR

    bool
        active = false,
        detected = false,
        pending = false;

    uint16_t
        level = 0,
        mic;

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
    bool play();

    void receiverHook(const JsonDocument doc) override;
};

extern MicrophoneExtension *Microphone;

#endif // EXTENSION_MICROPHONE
