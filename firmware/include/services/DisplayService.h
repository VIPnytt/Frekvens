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
    static constexpr uint8_t frameRate = FRAME_RATE;
#else
    static constexpr uint8_t frameRate = 60;
#endif // FRAME_RATE

    enum class Orientation : uint8_t // NOLINT(performance-enum-size)
    {
        deg0,
        deg90,
        deg180,
        deg270,
    };

    // NOLINTNEXTLINE(bugprone-throwing-static-initialization,cert-err58-cpp)
    inline static const uint8_t depth =
        min<uint8_t>(log2f(1 / PWM_WIDTH / static_cast<float>(frameRate * 2)), SOC_LEDC_TIMER_BIT_WIDTH);

    static constexpr std::array<uint16_t, 12> splash{
        0b1000001001,
        0b1000000001,
        0b1110001001,
        0b1001001001,
        0b1001001000,
        0b1001001001,
        0b0000000000,
        0b0011001100,
        0b0011001100,
        0b0000000000,
        0b0110000110,
        0b0011111100,
    };

    bool pending = false;
    bool power = false;

#if GRID_COLUMNS == GRID_ROWS && PITCH_HORIZONTAL != PITCH_VERTICAL
    float ratio = static_cast<float>(PITCH_HORIZONTAL) / static_cast<float>(PITCH_VERTICAL);
#else
    static constexpr float ratio = static_cast<float>(PITCH_HORIZONTAL) / static_cast<float>(PITCH_VERTICAL);
#endif // GRID_COLUMNS == GRID_ROWS && PITCH_HORIZONTAL == PITCH_VERTICAL

    uint8_t brightness = 0;

    std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> _frame{};
    std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> frame{};
    std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> pixel{LED_MAP};

    Orientation orientation = Orientation::deg0;

    void transmit();

    static void onPowerOff();

    static IRAM_ATTR void onTimer();

public:
    hw_timer_t *timer = nullptr;

    void configure();
    void begin();

    void handle();

    [[nodiscard]] float getRatio() const;

    [[nodiscard]] Orientation getOrientation() const;
    void setOrientation(Orientation _orientation);

    [[nodiscard]] bool getPower() const;
    void setPower(bool _power);

    [[nodiscard]] uint8_t getBrightness() const;
    void setBrightness(uint8_t _brightness);

    void getFrame(std::span<uint8_t> frameCurrent) const;
    void setFrame(std::span<const uint8_t> frameNext);

    void clearFrame(uint8_t _brightness = 0);
    void invertFrame();

    [[nodiscard]] uint8_t getPixel(uint8_t x, uint8_t y) const;
    void setPixel(uint8_t x, uint8_t y, uint8_t _brightness = UINT8_MAX);

    void drawEllipse(float x, float y, float radius, float _ratio = 1, bool fill = false,
                     uint8_t _brightness = UINT8_MAX);
    void drawRectangle(uint8_t minX, uint8_t minY, uint8_t maxX, uint8_t maxY, bool fill = true,
                       uint8_t _brightness = UINT8_MAX);

    void flush();

    void onReceive(JsonObjectConst payload, std::string_view source) override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif

    static DisplayService &getInstance();
};

extern DisplayService &Display; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
