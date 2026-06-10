#pragma once

#if MODE_WAVEFORM

#include "modules/ModeModule.h"

#include <array>
#include <span>

class WaveformMode final : public ModeModule
{
private:
    static constexpr std::array<uint16_t, 2U> jagged0{
        0b0010001000100100U,
        0b1101110111011011U,
    };
    static constexpr std::array<uint16_t, 5U> jagged1{
        0b0000001000100100U,
        0b0010001001101010U,
        0b1101010101011010U,
        0b0001010101000001U,
        0b0000100010000001U,
    };
    static constexpr std::array<uint16_t, 5U> jagged2{
        0b0010000100000000U,
        0b0101001010010000U,
        0b1000101010101001U,
        0b0000101010100101U,
        0b0000010001000111U,
    };
    static constexpr std::array<uint16_t, 9U> jagged3{
        0b0000000100010000U,
        0b0111000100010000U,
        0b1001001010101000U,
        0b1000101010101000U,
        0b1000101010101101U,
        0b0000101010100101U,
        0b0000101010100111U,
        0b0000011001000000U,
        0b0000010001000000U,
    };
    static constexpr std::array<uint16_t, 12U> jagged4{
        0b0000000000010000U,
        0b0000000000011000U,
        0b0000000000101100U,
        0b0010000000100100U,
        0b0101100000100100U,
        0b1000010000100100U,
        0b1000001000100100U,
        0b1000000100100100U,
        0b0000000100100100U,
        0b0000000101100100U,
        0b0000000101000111U,
        0b0000000010000010U,
    };
    static constexpr std::array<uint16_t, 12U> jagged5{
        0b0000000000010000U,
        0b0110000000011000U,
        0b1001000000110100U,
        0b1001000000100100U,
        0b1001000100100100U,
        0b1001001110100100U,
        0b1001001010100101U,
        0b0001001010100101U,
        0b0001001010100101U,
        0b0000101001000101U,
        0b0000110000000111U,
        0b0000010000000010U,
    };
    static constexpr std::array<uint16_t, 3U> smooth0{
        0b0100000000000000U,
        0b1010000000000000U,
        0b1001111111111111U,
    };
    static constexpr std::array<uint16_t, 4U> smooth1{
        0b1000000000000000U,
        0b0100000000000000U,
        0b0010110000000000U,
        0b0001011111111111U,
    };
    static constexpr std::array<uint16_t, 6U> smooth2{
        0b0000000000000110U,
        0b0001100000001001U,
        0b0010010000010000U,
        0b0100001000100000U,
        0b1000000100100000U,
        0b0000000011000000U,
    };
    static constexpr std::array<uint16_t, 6U> smooth3{
        0b0000000000011000U,
        0b0100000000100100U,
        0b1010000001000010U,
        0b0001000010000010U,
        0b0000100100000010U,
        0b0000011000000001U,
    };
    static constexpr std::array<uint16_t, 7U> smooth4{
        0b0000001100000000U,
        0b0000010010000000U,
        0b1000100010000000U,
        0b0101000001000100U,
        0b0010000000101011U,
        0b0000000000101000U,
        0b0000000000010000U,
    };
    static constexpr std::array<uint16_t, 7U> smooth5{
        0b0001100000000000U,
        0b0010100000000000U,
        0b0100100001000000U,
        0b1000100010111111U,
        0b0000100010000000U,
        0b0000010100000000U,
        0b0000001000000000U,
    };
    static constexpr std::array<std::span<const uint16_t>, 6U> jagged{
        jagged0,
        jagged1,
        jagged2,
        jagged3,
        jagged4,
        jagged5,
    };
    static constexpr std::array<std::span<const uint16_t>, 12U> mixed{
        jagged0,
        jagged1,
        jagged2,
        jagged3,
        jagged4,
        jagged5,
        smooth0,
        smooth1,
        smooth2,
        smooth3,
        smooth4,
        smooth5,
    };
    static constexpr std::array<std::span<const uint16_t>, 6U> smooth{
        smooth0,
        smooth1,
        smooth2,
        smooth3,
        smooth4,
        smooth5,
    };
    static constexpr std::array<std::span<const std::span<const uint16_t>>, 3U> waves{
        jagged,
        mixed,
        smooth,
    };
    static constexpr std::array<std::string_view, 3U> waveNames{
        "Jagged",
        "Mixed",
        "Smooth",
    };

    enum class Wave : uint8_t // NOLINT(performance-enum-size)
    {
        jagged,
        mixed,
        smooth,
    };

    static inline Wave wave{Wave::mixed};

    unsigned long lastMillis{0UL};

    void draw();
    void setWave(std::string_view waveName);

    void transmit();

public:
    static constexpr std::string_view name{"Waveform"};

    explicit WaveformMode() : ModeModule(name) {};

    void configure() override;
    void handle() override;

    void onReceive(JsonObjectConst payload, std::string_view source) override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif
};

#endif // MODE_WAVEFORM
