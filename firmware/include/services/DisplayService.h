#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>

#include "config/constants.h"
#include "modules/ServiceModule.h"

class DisplayService : public ServiceModule
{
private:
    DisplayService() : ServiceModule("Display") {};

    enum Orientation
    {
        deg0,
        deg90,
        deg180,
        deg270,
    };

    // https://www.hackster.io/news/philip-stapelfeldt-s-ikea-hack-turns-your-obegransad-wall-lamp-into-a-smart-display-and-more-15d20d2463d2
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

    bool
        pending = false,
        power = false;

#if COLUMNS == ROWS
    float
#else
    static constexpr float
#endif
        cellRatio = CELL_WIDTH / (float)CELL_HEIGHT,
        globalRatio = COLUMNS * CELL_WIDTH / (float)ROWS / (float)CELL_HEIGHT;

    uint8_t
        globalBrightness = UINT8_MAX,
        frameDraft[COLUMNS * ROWS] = {0},
        frameReady[COLUMNS * ROWS] = {0},
        pixelBitOrder[COLUMNS * ROWS] = PIXEL_ORDER;

    Orientation globalOrientation = Orientation::deg0;

    void transmit();

    static IRAM_ATTR void onTimer();

public:
    hw_timer_t *timer;

    void setup();
    void ready();

    void handle();

    double getCellRatio() const;

    Orientation getGlobalOrientation() const;
    void setGlobalOrientation(Orientation rotation);

    bool getPower() const;
    void setPower(bool state);

    uint8_t getGlobalBrightness() const;
    void setGlobalBrightness(uint8_t brightness);

    void clear(uint8_t brightness = 0);
    void invert();

    void getFrame(uint8_t frame[COLUMNS * ROWS]);
    void setFrame(uint8_t frame[COLUMNS * ROWS]);

    uint8_t getPixel(uint8_t x, uint8_t y) const;
    void setPixel(uint8_t x, uint8_t y, uint8_t brightness = UINT8_MAX);

    void drawEllipse(double x, double y, double radius, double ratio = 1, bool fill = false, uint8_t brightness = UINT8_MAX);
    void drawRectangle(uint8_t minX, uint8_t minY, uint8_t maxX, uint8_t maxY, bool fill = true, uint8_t brightness = UINT8_MAX);

    void flush();

    void receiverHook(const JsonDocument doc) override;

    static DisplayService &getInstance();
};

extern DisplayService &Display;
