#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "handlers/BitmapHandler.h"
#include "services/DisplayService.h"


template <typename T>
    requires std::is_unsigned_v<T>
BitmapHandler<T>::BitmapHandler(std::span<const T> bitmap) : bitmap(bitmap)
{
    if (!bitmap.empty())
    {
        uint8_t msbMax = 0; // NOLINT(misc-const-correctness)
        for (T bitset : bitmap)
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

template <typename T>
    requires std::is_unsigned_v<T>
void BitmapHandler<T>::draw(uint8_t brightness) const
{
    draw((GRID_COLUMNS - width) / 2, (GRID_ROWS - height) / 2, brightness);
}

template <typename T>
    requires std::is_unsigned_v<T>
void BitmapHandler<T>::draw(uint8_t x, uint8_t y, uint8_t brightness) const
{
    uint8_t msbMax = 0; // NOLINT(misc-const-correctness)
    for (T bitset : bitmap)
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
    for (T _x = 0; _x <= msbMax; ++_x)
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

template <typename T>
    requires std::is_unsigned_v<T>
uint8_t BitmapHandler<T>::getHeight() const
{
    return height;
}

template <typename T>
    requires std::is_unsigned_v<T>
uint8_t BitmapHandler<T>::getWidth() const
{
    return width;
}
