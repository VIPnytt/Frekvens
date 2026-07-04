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
    static constexpr std::string_view name{"Infrared"};

#ifdef DECODE_RC5
    static constexpr std::array<uint16_t, 1U> rc5DisplayBrightnessDecrease{0x11U}; // Philips: Volume-
    static constexpr std::array<uint16_t, 1U> rc5DisplayBrightnessIncrease{0x10U}; // Philips: Volume+
    static constexpr std::array<uint16_t, 1U> rc5DisplayPowerToggle{0xCU};         // Philips: Power
    static constexpr std::array<uint16_t, 1U> rc5ExtensionMicrophoneToggle{0xDU};  // Philips: Mute
    static constexpr std::array<uint16_t, 1U> rc5ExtensionPhotocellToggle{0x47U};  // Philips: Dim
    static constexpr std::array<uint16_t, 1U> rc5ExtensionPlaylistStart{0x35U};    // Philips: Play/pause
    static constexpr std::array<uint16_t, 1U> rc5ExtensionPlaylistStop{0x36U};     // Philips: Stop
    static constexpr std::array<uint16_t, 2U> rc5ModeNext{
        0x1EU, // Philips: Album next
        0x20U, // Philips: Title next
    };
    static constexpr std::array<uint16_t, 2U> rc5ModePrevious{
        0x1FU, // Philips: Album previous
        0x21U, // Philips: Title previous
    };
#endif // DECODE_RC5

#ifdef DECODE_SONY
    static constexpr std::array<uint16_t, 1U> sonyDisplayBrightnessDecrease{0x13U}; // Sony: Volume-
    static constexpr std::array<uint16_t, 1U> sonyDisplayBrightnessIncrease{0x12U}; // Sony: Volume+
    static constexpr std::array<uint16_t, 2U> sonyDisplayPowerToggle{
        0x15U,   // Sony: Power
        0x7115U, // Sony: Power
    };
    static constexpr std::array<uint16_t, 1U> sonyExtensionMicrophoneToggle{0x14U}; // Sony: Mute
    static constexpr std::array<uint16_t, 1U> sonyExtensionPhotocellToggle{0x78U};  // Sony: Scene
    static constexpr std::array<uint16_t, 1U> sonyExtensionPlaylistStart{0x711AU};  // Sony: Play
    static constexpr std::array<uint16_t, 2U> sonyExtensionPlaylistStop{
        0x7118U, // Sony: Stop
        0x7119U, // Sony: Pause
    };
    static constexpr std::array<uint16_t, 3U> sonyModeNext{
        0x10U,   // Sony: Program+
        0x711CU, // Sony: Fast forward
        0x7156U, // Sony: Next
    };
    static constexpr std::array<uint16_t, 3U> sonyModePrevious{
        0x11U,   // Sony: Program-
        0x711BU, // Sony: Rewind
        0x7157U, // Sony: Previous
    };
#endif // DECODE_SONY

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
    static constexpr std::array<Code, 2U> codes{rc5, sony};
#elif defined(DECODE_RC5)
    static constexpr std::array<Code, 1U> codes{rc5};
#elif defined(DECODE_SONY)
    static constexpr std::array<Code, 1U> codes{sony};
#endif // defined(DECODE_RC5) && defined(DECODE_SONY)

    bool active{false};

    unsigned long lastMillis{0UL};

    void transmit();

public:
    explicit InfraredExtension() : ExtensionModule(name) {};

    void configure() override;
    void begin() override;
    void handle() override;

    [[nodiscard]] bool getActive() const;
    void setActive(bool _active);
    void parse();

    void onReceive(JsonObjectConst payload, std::string_view source) override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif
};

#endif // EXTENSION_INFRARED
