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

    enum class Condition : uint8_t // NOLINT(performance-enum-size)
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

    virtual void update(std::optional<Condition> &condition, std::optional<int16_t> &temperature,
                        unsigned long &lastMillis) = 0;

    [[nodiscard]] std::variant<std::span<const uint8_t>, std::span<const uint16_t>> getSign(Condition condition);

protected:
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    explicit WeatherHandler(std::string_view name, unsigned long interval = 0b1U << 20U)
        : name(name), interval(interval) {};

    static constexpr std::array<uint8_t, 7U> bitmapClear{
        0b0011100U,
        0b0111110U,
        0b1111111U,
        0b1111111U,
        0b1111111U,
        0b0111110U,
        0b0011100U,
    };
    static constexpr std::array<uint8_t, 8U> bitmapClearTall{
        0b001100U,
        0b011110U,
        0b111111U,
        0b111111U,
        0b111111U,
        0b111111U,
        0b011110U,
        0b001100U,
    };
    static constexpr std::array<uint8_t, 6U> bitmapClearWide{
        0b00111100U,
        0b01111110U,
        0b11111111U,
        0b11111111U,
        0b01111110U,
        0b00111100U,
    };
    static constexpr std::array<uint16_t, 5U> bitmapCloudy{
        0b0000111000000U,
        0b0001100111000U,
        0b0111000001100U,
        0b1100100000011U,
        0b0111111111110U,
    };
    static constexpr std::array<uint16_t, 6U> bitmapCloudyPartly{
        0b0000000001110U,
        0b0000111011111U,
        0b0001100111111U,
        0b0111000001110U,
        0b1100010000011U,
        0b0111111111110U,
    };
    static constexpr std::array<uint16_t, 6U> bitmapExceptional{
        0b00000100000U,
        0b00001010000U,
        0b00010001000U,
        0b00100000100U,
        0b01000000010U,
        0b11111111111U,
    };
    static constexpr std::array<uint16_t, 6U> bitmapFog{
        0b00000111111110U,
        0b01111110000000U,
        0b00001111111111U,
        0b11111110000000U,
        0b00000111111100U,
        0b01111111000000U,
    };
    static constexpr std::array<uint16_t, 8U> bitmapRain{
        0b0000111000000U,
        0b0001100111000U,
        0b0111000001100U,
        0b1100100000011U,
        0b0111111111110U,
        0b0010100100100U,
        0b0010100100100U,
        0b0010100100100U,
    };
    static constexpr std::array<uint16_t, 8U> bitmapSnow{
        0b0000111000000U,
        0b0001100111000U,
        0b0111000001100U,
        0b1100100000011U,
        0b0111111111110U,
        0b0001010010000U,
        0b0010000100100U,
        0b0000100010000U,
    };
    static constexpr std::array<uint16_t, 8U> bitmapThunder{
        0b0000111000000U,
        0b0001100111000U,
        0b0111000001100U,
        0b1100000100011U,
        0b0111001001110U,
        0b0000011100000U,
        0b0000001000000U,
        0b0000010000000U,
    };
    static constexpr std::array<uint16_t, 8U> bitmapWind{
        0b10111000000U,
        0b10100111000U,
        0b11100000111U,
        0b10100111000U,
        0b10111000000U,
        0b10000000000U,
        0b10000000000U,
        0b10000000000U,
    };

    const char *host{nullptr};
    const char *path{nullptr};
    const char *query{nullptr};

    bool tls{true};

    uint16_t port{443U};

    std::vector<std::pair<const char *const, const char *const>> headers{
        {"Accept", "application/json"},
        {"Accept-Encoding", "identity"},
    };

    [[nodiscard]] int fetch(std::vector<char> &body, unsigned long &lastMillis);

    [[nodiscard]] std::optional<Condition>
    getCondition(std::string_view code,
                 std::span<const std::pair<Condition, std::span<const std::string_view>>> codesets);

    template <typename T>
        requires std::is_unsigned_v<T>
    [[nodiscard]] std::optional<Condition>
    getCondition(T code, std::span<const std::pair<Condition, std::span<const T>>> codesets);
};
