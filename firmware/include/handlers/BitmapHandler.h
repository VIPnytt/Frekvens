#pragma once

#include <stdint.h>
#include <vector>

class BitmapHandler
{
private:
    const std::vector<uint16_t> bitmap;

    uint8_t
        height = 0,
        width = 0;

public:
    BitmapHandler(std::vector<uint16_t> bitmap);

    void draw(uint8_t brightness = UINT8_MAX);
    void draw(uint8_t x, uint8_t y, uint8_t brightness = UINT8_MAX);
    uint8_t getHeight() const;
    uint8_t getWidth() const;
};
