#pragma once

#include "config/constants.h"
#include "modules/ServiceModule.h"

#include <vector>

class DisplayService final : public ServiceModule
{
private:
    explicit DisplayService() : ServiceModule("Display") {};

#ifdef FRAME_RATE
    static constexpr uint8_t frameRate = FRAME_RATE;
#else
    static constexpr uint8_t frameRate = 60;
#endif // FRAME_RATE

    enum Orientation
    {
        deg0,
        deg90,
        deg180,
        deg270,
    };

    const uint8_t depth =
        min<uint8_t>(log2f(1 / PWM_WIDTH / static_cast<float>(frameRate * 2)), SOC_LEDC_TIMER_BIT_WIDTH);

    const std::vector<uint16_t> hi = {
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
    uint8_t _frame[GRID_COLUMNS * GRID_ROWS] = {0};
    uint8_t frame[GRID_COLUMNS * GRID_ROWS] = {0};
    uint8_t pixel[GRID_COLUMNS * GRID_ROWS] = LED_MAP;

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

    void getFrame(uint8_t frameCurrent[GRID_COLUMNS * GRID_ROWS]);
    void setFrame(const uint8_t frameNext[GRID_COLUMNS * GRID_ROWS]);

    void clearFrame(uint8_t _brightness = 0);
    void invertFrame();

    [[nodiscard]] uint8_t getPixel(uint8_t x, uint8_t y) const;
    void setPixel(uint8_t x, uint8_t y, uint8_t _brightness = UINT8_MAX);

    void drawEllipse(float x, float y, float radius, float _ratio = 1, bool fill = false,
                     uint8_t _brightness = UINT8_MAX);
    void drawRectangle(uint8_t minX, uint8_t minY, uint8_t maxX, uint8_t maxY, bool fill = true,
                       uint8_t _brightness = UINT8_MAX);

    void flush();

    void onReceive(JsonObjectConst payload, const char *source) override;

    static DisplayService &getInstance();
};

extern DisplayService &Display;
