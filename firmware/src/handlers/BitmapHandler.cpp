#include "handlers/BitmapHandler.h"

#include "config/constants.h"
#include "services/DisplayService.h"

BitmapHandler::BitmapHandler(std::vector<uint16_t> bitmap) : bitmap(bitmap)
{
    if (!bitmap.empty())
    {
        uint8_t msbMax = 0;
        for (uint16_t bitset : bitmap)
        {
            uint8_t msb = 0;
            while (bitset >>= 1)
            {
                ++msb;
            }
            if (msb > msbMax)
            {
                msbMax = msb;
            }
        }
        height = bitmap.size();
        width = msbMax + 1;
    }
}

void BitmapHandler::draw(uint8_t brightness)
{
    draw((GRID_COLUMNS - width) / 2, (GRID_ROWS - height) / 2, brightness);
}

void BitmapHandler::draw(uint8_t x, uint8_t y, uint8_t brightness)
{
    uint8_t msbMax = 0;
    for (uint16_t bitset : bitmap)
    {
        uint8_t msb = 0;
        while (bitset >>= 1)
        {
            ++msb;
        }
        if (msb > msbMax)
        {
            msbMax = msb;
        }
    }
    for (uint8_t _x = 0; _x <= msbMax; ++_x)
    {
        for (uint8_t _y = 0; _y < height; ++_y)
        {
            if ((bitmap[_y] >> (msbMax - _x)) & 1)
            {
                Display.setPixel(x + _x, y + _y, brightness);
            }
        }
    }
}

uint8_t BitmapHandler::getHeight() const
{
    return height;
}

uint8_t BitmapHandler::getWidth() const
{
    return width;
}
