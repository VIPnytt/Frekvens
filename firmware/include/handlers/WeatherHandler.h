#pragma once

#include <string>
#include <vector>

class WeatherHandler
{
public:
    explicit WeatherHandler();

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
        const std::vector<std::string> codes = {};
    };

    struct Codeset8
    {
        const Conditions condition;
        const std::vector<uint8_t> codes = {};
    };

    struct Codeset16
    {
        const Conditions condition;
        const std::vector<uint16_t> codes = {};
    };

    std::vector<uint16_t> sign = {};
    int16_t temperature = 0;

    void parse(std::string code, std::vector<Codeset> codesets);
    void parse(uint8_t code, std::vector<Codeset8> codesets);
    void parse(uint16_t code, std::vector<Codeset16> codesets);
    void draw();

private:
    static constexpr std::string_view _name = "WeatherHandler";

    const std::vector<uint16_t> conditionClear = {
        0b0011100,
        0b0111110,
        0b1111111,
        0b1111111,
        0b1111111,
        0b0111110,
        0b0011100,
    };
    const std::vector<uint16_t> conditionClearTall = {
        0b001100,
        0b011110,
        0b111111,
        0b111111,
        0b111111,
        0b111111,
        0b011110,
        0b001100,
    };
    const std::vector<uint16_t> conditionClearWide = {
        0b00111100,
        0b01111110,
        0b11111111,
        0b11111111,
        0b01111110,
        0b00111100,
    };
    const std::vector<uint16_t> conditionCloudy = {
        0b0000111000000,
        0b0001100111000,
        0b0111000001100,
        0b1100100000011,
        0b0111111111110,
    };
    const std::vector<uint16_t> conditionCloudyPartly = {
        0b0000000001110,
        0b0000111011111,
        0b0001100111111,
        0b0111000001110,
        0b1100010000011,
        0b0111111111110,
    };
    const std::vector<uint16_t> conditionExceptional = {
        0b00000100000,
        0b00001010000,
        0b00010001000,
        0b00100000100,
        0b01000000010,
        0b11111111111,
    };
    const std::vector<uint16_t> conditionFog = {
        0b00000111111110,
        0b01111110000000,
        0b00001111111111,
        0b11111110000000,
        0b00000111111100,
        0b01111111000000,
    };
    const std::vector<uint16_t> conditionRain = {
        0b0000111000000,
        0b0001100111000,
        0b0111000001100,
        0b1100100000011,
        0b0111111111110,
        0b0010100100100,
        0b0010100100100,
        0b0010100100100,
    };
    const std::vector<uint16_t> conditionSnow = {
        0b0000111000000,
        0b0001100111000,
        0b0111000001100,
        0b1100100000011,
        0b0111111111110,
        0b0001010010000,
        0b0010000100100,
        0b0000100010000,
    };
    const std::vector<uint16_t> conditionThunder = {
        0b0000111000000,
        0b0001100111000,
        0b0111000001100,
        0b1100000100011,
        0b0111001001110,
        0b0000011100000,
        0b0000001000000,
        0b0000010000000,
    };
    const std::vector<uint16_t> conditionWind = {
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
