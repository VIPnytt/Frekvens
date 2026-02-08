#pragma once

#if EXTENSION_INFRARED

#include "config/constants.h"
#include "modules/ExtensionModule.h"

#define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#include <IRremote.hpp>
#undef USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#undef _IR_REMOTE_HPP

#include <vector>

class InfraredExtension : public ExtensionModule
{
private:
    struct Code
    {
        const decode_type_t protocol;
        const std::vector<uint16_t> displayBrightnessDecrease, displayBrightnessIncrease, displayPowerToggle,
#if EXTENSION_MICROPHONE
            extensionMicrophoneToggle,
#endif // EXTENSION_MICROPHONE
#if EXTENSION_PHOTOCELL
            extensionPhotocellToggle,
#endif // EXTENSION_PHOTOCELL
#if EXTENSION_PLAYLIST
            extensionPlaylistStart, extensionPlaylistStop,
#endif // EXTENSION_PLAYLIST
            modeNext, modePrevious;
    };

    const std::vector<Code> codes = {
#ifdef DECODE_RC5
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
            {
                0x47, // Philips: Dim
            },
#endif // EXTENSION_PHOTOCELL
#if EXTENSION_PLAYLIST
            {
                0x35, // Philips: Play/pause
            },
            {
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
#endif // DECODE_RC5
#ifdef DECODE_SONY
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
                0x711A, // Sony: Play
            },
            {
                0x7118, // Sony: Stop
                0x7119, // Sony: Pause
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

    bool active = false;

    unsigned long lastMillis = 0;

    void transmit();

public:
    InfraredExtension();

    void configure() override;
    void begin() override;
    void handle() override;

    bool getActive();
    void setActive(bool active);
    void parse();

    void onReceive(const JsonDocument doc, const char *const source) override;
};

extern InfraredExtension *Infrared;

#endif // EXTENSION_INFRARED
