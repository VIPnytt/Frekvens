#pragma once

#if EXTENSION_INFRARED

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ExtensionModule.h"

#define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#include <IRremote.hpp>
#undef USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#undef _IR_REMOTE_HPP

#include <array>
#include <span>

class InfraredExtension final : public ExtensionModule
{
private:
    struct Code
    {
        decode_type_t protocol{};
        std::span<const uint16_t> displayBrightnessDecrease{};
        std::span<const uint16_t> displayBrightnessIncrease{};
        std::span<const uint16_t> displayPowerToggle{};
#if EXTENSION_MICROPHONE
        std::span<const uint16_t> extensionMicrophoneToggle{};
#endif // EXTENSION_MICROPHONE
#if EXTENSION_PHOTOCELL
        std::span<const uint16_t> extensionPhotocellToggle{};
#endif // EXTENSION_PHOTOCELL
#if EXTENSION_PLAYLIST
        std::span<const uint16_t> extensionPlaylistStart{};
        std::span<const uint16_t> extensionPlaylistStop{};
#endif // EXTENSION_PLAYLIST
        std::span<const uint16_t> modeNext{};
        std::span<const uint16_t> modePrevious{};
    };

#ifdef DECODE_RC5
    static constexpr std::array<uint16_t, 1> rc5DisplayBrightnessDecrease{0x11}; // Philips: Volume-
    static constexpr std::array<uint16_t, 1> rc5DisplayBrightnessIncrease{0x10}; // Philips: Volume+
    static constexpr std::array<uint16_t, 1> rc5DisplayPowerToggle{0xC};         // Philips: Power
    static constexpr std::array<uint16_t, 1> rc5ExtensionMicrophoneToggle{0xD};  // Philips: Mute
    static constexpr std::array<uint16_t, 1> rc5ExtensionPhotocellToggle{0x47};  // Philips: Dim
    static constexpr std::array<uint16_t, 1> rc5ExtensionPlaylistStart{0x35};    // Philips: Play/pause
    static constexpr std::array<uint16_t, 1> rc5ExtensionPlaylistStop{0x36};     // Philips: Stop
    static constexpr std::array<uint16_t, 2> rc5ModeNext{
        0x1E, // Philips: Album next
        0x20, // Philips: Title next
    };
    static constexpr std::array<uint16_t, 2> rc5ModePrevious{
        0x1F, // Philips: Album previous
        0x21, // Philips: Title previous
    };
#endif // DECODE_RC5

#ifdef DECODE_SONY
    static constexpr std::array<uint16_t, 1> sonyDisplayBrightnessDecrease{0x13}; // Sony: Volume-
    static constexpr std::array<uint16_t, 1> sonyDisplayBrightnessIncrease{0x12}; // Sony: Volume+
    static constexpr std::array<uint16_t, 2> sonyDisplayPowerToggle{
        0x15,   // Sony: Power
        0x7115, // Sony: Power
    };
    static constexpr std::array<uint16_t, 1> sonyExtensionMicrophoneToggle{0x14}; // Sony: Mute
    static constexpr std::array<uint16_t, 1> sonyExtensionPhotocellToggle{0x78};  // Sony: Scene
    static constexpr std::array<uint16_t, 1> sonyExtensionPlaylistStart{0x711A};  // Sony: Play
    static constexpr std::array<uint16_t, 2> sonyExtensionPlaylistStop{
        0x7118, // Sony: Stop
        0x7119, // Sony: Pause
    };
    static constexpr std::array<uint16_t, 3> sonyModeNext{
        0x10,   // Sony: Program+
        0x711C, // Sony: Fast forward
        0x7156, // Sony: Next
    };
    static constexpr std::array<uint16_t, 3> sonyModePrevious{
        0x11,   // Sony: Program-
        0x711B, // Sony: Rewind
        0x7157, // Sony: Previous
    };
#endif // DECODE_SONY

#ifdef DECODE_RC5
    static constexpr Code rc5{
        decode_type_t::RC5,
        rc5DisplayBrightnessDecrease,
        rc5DisplayBrightnessIncrease,
        rc5DisplayPowerToggle,
#if EXTENSION_MICROPHONE
        rc5ExtensionMicrophoneToggle,
#endif
#if EXTENSION_PHOTOCELL
        rc5ExtensionPhotocellToggle,
#endif
#if EXTENSION_PLAYLIST
        rc5ExtensionPlaylistStart,
        rc5ExtensionPlaylistStop,
#endif
        rc5ModeNext,
        rc5ModePrevious,
    };
#endif // DECODE_RC5

#ifdef DECODE_SONY
    static constexpr Code sony{
        decode_type_t::SONY,
        sonyDisplayBrightnessDecrease,
        sonyDisplayBrightnessIncrease,
        sonyDisplayPowerToggle,
#if EXTENSION_MICROPHONE
        sonyExtensionMicrophoneToggle,
#endif
#if EXTENSION_PHOTOCELL
        sonyExtensionPhotocellToggle,
#endif
#if EXTENSION_PLAYLIST
        sonyExtensionPlaylistStart,
        sonyExtensionPlaylistStop,
#endif
        sonyModeNext,
        sonyModePrevious,
    };
#endif // DECODE_SONY

#if defined(DECODE_RC5) && defined(DECODE_SONY)
    static constexpr std::array<Code, 2> codes{rc5, sony};
#elif defined(DECODE_RC5)
    static constexpr std::array<Code, 1> codes{rc5};
#elif defined(DECODE_SONY)
    static constexpr std::array<Code, 1> codes{sony};
#endif // defined(DECODE_RC5) && defined(DECODE_SONY)

    bool active = false;

    unsigned long lastMillis = 0;

    void transmit();

public:
    explicit InfraredExtension();

    void configure() override;
    void begin() override;
    void handle() override;

    [[nodiscard]] bool getActive() const;
    void setActive(bool active);
    void parse();

    void onReceive(JsonObjectConst payload, const char *source) override;
};

extern InfraredExtension *Infrared; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

#endif // EXTENSION_INFRARED
