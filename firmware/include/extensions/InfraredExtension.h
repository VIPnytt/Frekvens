#pragma once

#include "config/constants.h"

#if EXTENSION_INFRARED

#include <IRrecv.h>
#include <vector>

#include "modules/ExtensionModule.h"

class InfraredExtension : public ExtensionModule
{
private:
    struct Code
    {
        const decode_type_t protocol;
        const std::vector<uint32_t>
            displayBrightnessDecrease,
            displayBrightnessIncrease,
            displayPowerToggle,
#if EXTENSION_MICROPHONE
            extensionMicrophoneToggle,
#endif // EXTENSION_MICROPHONE
#if EXTENSION_PHOTOCELL
            extensionPhotocellToggle,
#endif // EXTENSION_PHOTOCELL
#if EXTENSION_PLAYLIST
            extensionPlaylistToggle,
#endif // EXTENSION_PLAYLIST
            modeNext,
            modePrevious;
    };

    const std::vector<Code> codes = {
#if DECODE_RC5
        {
            decode_type_t::RC5,
            {
                0x11, // Philips: Volume-
            },
            {
                0x10, // Philips: Volume+
            },
            {
                0xC, // Philips: Power
            },
#if EXTENSION_MICROPHONE
            {
                0xD, // Philips: Mute
            },
#endif // EXTENSION_MICROPHONE
#if EXTENSION_PHOTOCELL
            {},
#endif // EXTENSION_PHOTOCELL
#if EXTENSION_PLAYLIST
            {
                0x35, // Philips: Play/pause
                0x36, // Philips: Stop
            },
#endif // EXTENSION_PLAYLIST
            {
                0x1E, // Philips: Album next
                0x20, // Philips: Title next
            },
            {
                0x1F, // Philips: Album previous
                0x21, // Philips: Title previous
            },
        },
        {
            decode_type_t::RC5X,
            {},
            {},
            {},
#if EXTENSION_MICROPHONE
            {},
#endif // EXTENSION_MICROPHONE
#if EXTENSION_PHOTOCELL
            {
                0x47, // Philips: Dim
            },
#endif // EXTENSION_PHOTOCELL
#if EXTENSION_PLAYLIST
            {},
#endif // EXTENSION_PLAYLIST
            {},
            {},
        },
#endif // DECODE_RC5
#if DECODE_SONY
        {
            decode_type_t::SONY,
            {
                0x13, // Sony: Volume-
            },
            {
                0x12, // Sony: Volume+
            },
            {
                0x15,   // Sony: Power
                0x7115, // Sony: Power
            },
#if EXTENSION_MICROPHONE
            {
                0x14, // Sony: Mute
            },
#endif // EXTENSION_MICROPHONE
#if EXTENSION_PHOTOCELL
            {
                0x78, // Sony: Scene
            },
#endif // EXTENSION_PHOTOCELL
#if EXTENSION_PLAYLIST
            {
                0x7118, // Sony: Stop
                0x7119, // Sony: Pause
                0x711A, // Sony: Play
            },
#endif // EXTENSION_PLAYLIST
            {
                0x10,   // Sony: Program+
                0x711C, // Sony: Fast forward
                0x7156, // Sony: Next
            },
            {
                0x11,   // Sony: Program-
                0x711B, // Sony: Rewind
                0x7157, // Sony: Previous
            },
        },
#endif // DECODE_SONY
    };

    IRrecv *irrecv;

    bool active = false;

    unsigned long lastMillis = 0;

    void transmit();

public:
    InfraredExtension();

    void setup() override;
    void ready() override;
    void handle() override;

    bool get();
    void set(bool enable);
    bool parse(decode_results results);

    void receiverHook(const JsonDocument doc) override;
};

extern InfraredExtension *Infrared;

#endif // EXTENSION_INFRARED
