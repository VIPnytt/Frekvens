#pragma once

#include <stdint.h>
#include <vector>

#include "config/constants.h"

class WeatherHandler
{
public:
    enum Conditions
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
        const Conditions condition;
        const std::vector<std::string> codes;
    };

    struct Codeset8
    {
        const Conditions condition;
        const std::vector<uint8_t> codes;
    };

    struct Codeset16
    {
        const Conditions condition;
        const std::vector<uint16_t> codes;
    };

    std::vector<uint16_t> sign = {};
    int16_t temperature;

    void parse(const std::string code, const std::vector<Codeset> codesets);
    void parse(const uint8_t code, const std::vector<Codeset8> codesets);
    void parse(const uint16_t code, const std::vector<Codeset16> codesets);
    void draw();

private:
    static constexpr std::string_view name = "WeatherHandler";

    const std::vector<uint16_t>
        conditionClear = {
            0b0011100,
            0b0111110,
            0b1111111,
            0b1111111,
            0b1111111,
            0b0111110,
            0b0011100,
    },
        conditionClearTall = {
            0b001100,
            0b011110,
            0b111111,
            0b111111,
            0b111111,
            0b111111,
            0b011110,
            0b001100,
    },
        conditionClearWide = {
            0b00111100,
            0b01111110,
            0b11111111,
            0b11111111,
            0b01111110,
            0b00111100,
    },
        conditionCloudy = {
            0b0000111000000,
            0b0001100111000,
            0b0111000001100,
            0b1100100000011,
            0b0111111111110,
    },
        conditionCloudyPartly = {
            0b0000000001110,
            0b0000111011111,
            0b0001100111111,
            0b0111000001110,
            0b1100010000011,
            0b0111111111110,
    },
        conditionExceptional = {
            0b00000100000,
            0b00001010000,
            0b00010001000,
            0b00100000100,
            0b01000000010,
            0b11111111111,
    },
        conditionFog = {
            0b00000111111110,
            0b01111110000000,
            0b00001111111111,
            0b11111110000000,
            0b00000111111100,
            0b01111111000000,
    },
        conditionRain = {
            0b0000111000000,
            0b0001100111000,
            0b0111000001100,
            0b1100100000011,
            0b0111111111110,
            0b0010100100100,
            0b0010100100100,
            0b0010100100100,
    },
        conditionSnow = {
            0b0000111000000,
            0b0001100111000,
            0b0111000001100,
            0b1100100000011,
            0b0111111111110,
            0b0001010010000,
            0b0010000100100,
            0b0000100010000,
    },
        conditionThunder = {
            0b0000111000000,
            0b0001100111000,
            0b0111000001100,
            0b1100000100011,
            0b0111001001110,
            0b0000011100000,
            0b0000001000000,
            0b0000010000000,
    },
        conditionWind = {
            0b10111000000,
            0b10100111000,
            0b11100000111,
            0b10100111000,
            0b10111000000,
            0b10000000000,
            0b10000000000,
            0b10000000000,
    };

    void setSign(Conditions condition);
};
