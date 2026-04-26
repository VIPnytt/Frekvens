#pragma once

#include <optional>
#include <span>
#include <string_view>
#include <variant>
#include <vector>

class WeatherHandler
{
public:
    virtual ~WeatherHandler() = default;

    WeatherHandler(const WeatherHandler &) = delete;
    WeatherHandler &operator=(const WeatherHandler &) = delete;
    WeatherHandler(WeatherHandler &&) = delete;
    WeatherHandler &operator=(WeatherHandler &&) = delete;

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

    const std::string_view name{};

    const unsigned long interval;

    virtual void update(std::optional<Conditions> &condition, std::optional<int16_t> &temperature,
                        unsigned long &lastMillis) = 0;

    [[nodiscard]] std::variant<std::span<const uint8_t>, std::span<const uint16_t>> getSign(Conditions condition);

protected:
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    explicit WeatherHandler(std::string_view name, unsigned long interval = 1U << 20U)
        : name(name), interval(interval) {};

    static constexpr std::array<uint8_t, 7> bitmapClear{
        0b0011100,
        0b0111110,
        0b1111111,
        0b1111111,
        0b1111111,
        0b0111110,
        0b0011100,
    };
    static constexpr std::array<uint8_t, 8> bitmapClearTall{
        0b001100,
        0b011110,
        0b111111,
        0b111111,
        0b111111,
        0b111111,
        0b011110,
        0b001100,
    };
    static constexpr std::array<uint8_t, 6> bitmapClearWide{
        0b00111100,
        0b01111110,
        0b11111111,
        0b11111111,
        0b01111110,
        0b00111100,
    };
    static constexpr std::array<uint16_t, 5> bitmapCloudy{
        0b0000111000000,
        0b0001100111000,
        0b0111000001100,
        0b1100100000011,
        0b0111111111110,
    };
    static constexpr std::array<uint16_t, 6> bitmapCloudyPartly{
        0b0000000001110,
        0b0000111011111,
        0b0001100111111,
        0b0111000001110,
        0b1100010000011,
        0b0111111111110,
    };
    static constexpr std::array<uint16_t, 6> bitmapExceptional{
        0b00000100000,
        0b00001010000,
        0b00010001000,
        0b00100000100,
        0b01000000010,
        0b11111111111,
    };
    static constexpr std::array<uint16_t, 6> bitmapFog{
        0b00000111111110,
        0b01111110000000,
        0b00001111111111,
        0b11111110000000,
        0b00000111111100,
        0b01111111000000,
    };
    static constexpr std::array<uint16_t, 8> bitmapRain{
        0b0000111000000,
        0b0001100111000,
        0b0111000001100,
        0b1100100000011,
        0b0111111111110,
        0b0010100100100,
        0b0010100100100,
        0b0010100100100,
    };
    static constexpr std::array<uint16_t, 8> bitmapSnow{
        0b0000111000000,
        0b0001100111000,
        0b0111000001100,
        0b1100100000011,
        0b0111111111110,
        0b0001010010000,
        0b0010000100100,
        0b0000100010000,
    };
    static constexpr std::array<uint16_t, 8> bitmapThunder{
        0b0000111000000,
        0b0001100111000,
        0b0111000001100,
        0b1100000100011,
        0b0111001001110,
        0b0000011100000,
        0b0000001000000,
        0b0000010000000,
    };
    static constexpr std::array<uint16_t, 8> bitmapWind{
        0b10111000000,
        0b10100111000,
        0b11100000111,
        0b10100111000,
        0b10111000000,
        0b10000000000,
        0b10000000000,
        0b10000000000,
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

    const char *host = nullptr;
    const char *path = nullptr;
    const char *query = nullptr;

    bool tls = true;

    uint16_t port = 443;

    std::vector<std::pair<const char *const, const char *const>> headers = {
        {"Accept", "application/json"},
    };

    [[nodiscard]] int fetch(std::vector<char> &body, unsigned long &lastMillis);

    [[nodiscard]] std::optional<Conditions> getCondition(std::string_view code, std::span<const Codeset> codesets);
    [[nodiscard]] std::optional<Conditions> getCondition(uint8_t code, std::span<const Codeset8> codesets);
    [[nodiscard]] std::optional<Conditions> getCondition(uint16_t code, std::span<const Codeset16> codesets);
};
