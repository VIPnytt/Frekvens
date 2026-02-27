#pragma once

#include "modules/HandlerModule.h"

#include <span>
#include <string>

class WeatherHandler final : public HandlerModule
{
public:
    WeatherHandler() = default;

    enum class Conditions : uint8_t // NOLINT(performance-enum-size)
    {
        CLEAR,
        CLOUDY,
        CLOUDY_PARTLY,
        EXCEPTION,
        FOG,
        RAIN,
        SNOW,
        THUNDER,
        WIND,
    };

    struct Codeset
    {
        Conditions condition{};
        std::span<const std::string_view> codes{};
    };

    struct Codeset8
    {
        Conditions condition{};
        std::span<const uint8_t> codes{};
    };

    struct Codeset16
    {
        Conditions condition{};
        std::span<const uint16_t> codes{};
    };

    int16_t temperature = 0;

    void parse(std::string_view code, std::span<const Codeset> codesets);
    void parse(uint8_t code, std::span<const Codeset8> codesets);
    void parse(uint16_t code, std::span<const Codeset16> codesets);
    void draw();

private:
    static constexpr std::string_view _name = "WeatherHandler";

    static constexpr std::array<uint16_t, 7> conditionClear{
        0b0011100,
        0b0111110,
        0b1111111,
        0b1111111,
        0b1111111,
        0b0111110,
        0b0011100,
    };
    static constexpr std::array<uint16_t, 8> conditionClearTall{
        0b001100,
        0b011110,
        0b111111,
        0b111111,
        0b111111,
        0b111111,
        0b011110,
        0b001100,
    };
    static constexpr std::array<uint16_t, 6> conditionClearWide{
        0b00111100,
        0b01111110,
        0b11111111,
        0b11111111,
        0b01111110,
        0b00111100,
    };
    static constexpr std::array<uint16_t, 5> conditionCloudy{
        0b0000111000000,
        0b0001100111000,
        0b0111000001100,
        0b1100100000011,
        0b0111111111110,
    };
    static constexpr std::array<uint16_t, 6> conditionCloudyPartly{
        0b0000000001110,
        0b0000111011111,
        0b0001100111111,
        0b0111000001110,
        0b1100010000011,
        0b0111111111110,
    };
    static constexpr std::array<uint16_t, 6> conditionExceptional{
        0b00000100000,
        0b00001010000,
        0b00010001000,
        0b00100000100,
        0b01000000010,
        0b11111111111,
    };
    static constexpr std::array<uint16_t, 6> conditionFog{
        0b00000111111110,
        0b01111110000000,
        0b00001111111111,
        0b11111110000000,
        0b00000111111100,
        0b01111111000000,
    };
    static constexpr std::array<uint16_t, 8> conditionRain{
        0b0000111000000,
        0b0001100111000,
        0b0111000001100,
        0b1100100000011,
        0b0111111111110,
        0b0010100100100,
        0b0010100100100,
        0b0010100100100,
    };
    static constexpr std::array<uint16_t, 8> conditionSnow{
        0b0000111000000,
        0b0001100111000,
        0b0111000001100,
        0b1100100000011,
        0b0111111111110,
        0b0001010010000,
        0b0010000100100,
        0b0000100010000,
    };
    static constexpr std::array<uint16_t, 8> conditionThunder{
        0b0000111000000,
        0b0001100111000,
        0b0111000001100,
        0b1100000100011,
        0b0111001001110,
        0b0000011100000,
        0b0000001000000,
        0b0000010000000,
    };
    static constexpr std::array<uint16_t, 8> conditionWind{
        0b10111000000,
        0b10100111000,
        0b11100000111,
        0b10100111000,
        0b10111000000,
        0b10000000000,
        0b10000000000,
        0b10000000000,
    };

    std::span<const uint16_t> sign{};

    void setSign(Conditions condition);
};
