#pragma once

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ServiceModule.h"

#include <array>
#include <span>

class DisplayService final : public ServiceModule
{
private:
    explicit DisplayService() : ServiceModule("Display") {};

#ifdef FRAME_RATE
    static_assert(FRAME_RATE >= 1U);
    static constexpr uint8_t fps{FRAME_RATE};
#else
    static constexpr uint8_t fps{F_CPU / 13'000U / (GRID_COLUMNS * GRID_ROWS)};
#endif // FRAME_RATE

#ifdef PWM_DEPTH
    static_assert(PWM_DEPTH <= SOC_LEDC_TIMER_BIT_WIDTH);
    static constexpr uint8_t depth{PWM_DEPTH};
#else
    // NOLINTNEXTLINE(bugprone-throwing-static-initialization,cert-err58-cpp)
    static inline const uint8_t depth{
        min(max<uint8_t>(
                8U, static_cast<uint8_t>(8.0F - (std::numbers::pi_v<float> * log2f(static_cast<float>(fps) / 120.0F)))),
            min<uint8_t>(SOC_LEDC_TIMER_BIT_WIDTH, std::ilogbf(1.0F / PWM_WIDTH / static_cast<float>(fps))))};
#endif // PWM_DEPTH

#if GRID_COLUMNS == GRID_ROWS && PITCH_HORIZONTAL != PITCH_VERTICAL
    float ratio{static_cast<float>(PITCH_HORIZONTAL) / static_cast<float>(PITCH_VERTICAL)};
#else
    static constexpr float ratio{static_cast<float>(PITCH_HORIZONTAL) / static_cast<float>(PITCH_VERTICAL)};
#endif // GRID_COLUMNS == GRID_ROWS && PITCH_HORIZONTAL != PITCH_VERTICAL

    static constexpr std::array<uint16_t, 12U> splash{
        0b1000001001U,
        0b1000000001U,
        0b1110001001U,
        0b1001001001U,
        0b1001001000U,
        0b1001001001U,
        0b0000000000U,
        0b0011001100U,
        0b0011001100U,
        0b0000000000U,
        0b0110000110U,
        0b0011111100U,
    };

    enum class Orientation : uint8_t // NOLINT(performance-enum-size)
    {
        deg0,
        deg90,
        deg180,
        deg270,
    };

    static inline DRAM_ATTR std::array<std::array<uint8_t, ((GRID_COLUMNS * GRID_ROWS) + 7U) / 8U>, UINT8_MAX> planes{};

    bool pending{false};
    bool power{false};
    bool render{false};

    uint8_t brightness{0U};

    std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> frame{};

#if GRID_COLUMNS * GRID_ROWS <= 0b1U << 8U
    std::array<std::pair<uint8_t, uint8_t>, GRID_COLUMNS * GRID_ROWS> pixelsMapped{};
#else
    std::array<std::pair<uint16_t, uint8_t>, GRID_COLUMNS * GRID_ROWS> pixelsMapped{};
#endif // GRID_COLUMNS * GRID_ROWS <= 0b1U << 8U

    Orientation orientation{Orientation::deg0};

#if GRID_COLUMNS * GRID_ROWS <= 0b1U << 8U
    void mapPixel(uint8_t logical, uint8_t physical);
#else
    void mapPixel(uint16_t logical, uint16_t physical);
#endif // GRID_COLUMNS * GRID_ROWS <= 0b1U << 8U

    void transmit();

    static void onPowerOff();

    static IRAM_ATTR void onTimer();

public:
#if GRID_COLUMNS * GRID_ROWS <= 0b1U << 8U
    static constexpr std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> pixels{LED_MAP};
#else
    static constexpr std::array<uint16_t, GRID_COLUMNS * GRID_ROWS> pixels{LED_MAP};
#endif // GRID_COLUMNS * GRID_ROWS <= 0b1U << 8U

    void configure();
    void handle();

    [[nodiscard]] float getRatio() const;

    [[nodiscard]] Orientation getOrientation() const;
    void setOrientation(Orientation _orientation);

    [[nodiscard]] bool getPower() const;
    void setPower(bool _power);

    [[nodiscard]] uint8_t getBrightness() const;
    void setBrightness(uint8_t _brightness);

    void getFrame(std::span<uint8_t, GRID_COLUMNS * GRID_ROWS> _frame) const;
    void setFrame(std::span<const uint8_t, GRID_COLUMNS * GRID_ROWS> _frame);

    [[nodiscard]] uint8_t getPixel(size_t idx) const;
    [[nodiscard]] uint8_t getPixel(uint8_t x, uint8_t y) const;

    void setPixel(size_t idx, uint8_t _brightness);
    void setPixel(uint8_t x, uint8_t y, uint8_t _brightness);

    void drawEllipseOutline(float x, float y, float radius, uint8_t _brightness);
    void drawEllipseSolid(float x, float y, float radius, uint8_t _brightness);

    void drawRectangleOutline(size_t minX, size_t columns, size_t minY, size_t maxY, uint8_t _brightness);
    void drawRectangleSolid(size_t minX, size_t columns, size_t minY, size_t maxY, uint8_t _brightness);

    void drawLineHorizontal(size_t xMin, size_t columns, size_t y, uint8_t _brightness);
    void drawLineVertical(uint8_t x, uint8_t yMin, uint8_t yMax, uint8_t _brightness);

    void fillColumn(uint8_t x, uint8_t _brightness);

    void fillFrame(uint8_t _brightness);

    void fillRow(size_t y, uint8_t _brightness);
    void fillRows(size_t minY, size_t rows, uint8_t _brightness);

    void flush();

    void invertFrame();

    void onReceive(JsonObjectConst payload, std::string_view source) override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif

    static DisplayService &getInstance();
};

extern DisplayService &Display; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
