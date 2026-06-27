#pragma once

#include "modules/HandlerModule.h" // NOLINT(misc-include-cleaner)

#include <span>
#include <string_view>

template <typename T>
    requires std::is_unsigned_v<T>
class BitmapHandler;

template <typename T> BitmapHandler(std::span<const T>) -> BitmapHandler<T>;

template <typename T, size_t N> BitmapHandler(const std::array<T, N> &) -> BitmapHandler<std::remove_cv_t<T>>;

template <typename T, size_t N> BitmapHandler(const T (&)[N]) -> BitmapHandler<std::remove_cv_t<T>>;

template <typename T>
    requires std::is_unsigned_v<T>
class BitmapHandler final : public HandlerModule
{
private:
    std::span<const T> bitmap{};

    uint8_t height{0U};
    uint8_t width{0U};

public:
    explicit BitmapHandler(std::span<const T> bitmap);

    void draw(uint8_t brightness = UINT8_MAX) const;
    void draw(uint8_t x, uint8_t y, uint8_t brightness = UINT8_MAX) const;
    [[nodiscard]] uint8_t getHeight() const;
    [[nodiscard]] uint8_t getWidth() const;
};

template <typename T>
    requires std::is_unsigned_v<T>
BitmapHandler<T>::BitmapHandler(std::span<const T> bitmap) : bitmap(bitmap)
{
    if (!bitmap.empty())
    {
        uint8_t msbMax{0U}; // NOLINT(misc-const-correctness)
        for (T bitset : bitmap)
        {
            uint8_t msb{0U};
            while (bitset >>= 1U)
            {
                ++msb;
            }
            if (msb > msbMax)
            {
                msbMax = msb;
            }
        }
        height = bitmap.size();
        width = msbMax + 1U;
    }
}

template <typename T>
    requires std::is_unsigned_v<T>
void BitmapHandler<T>::draw(uint8_t brightness) const
{
    draw((GRID_COLUMNS - width) / 2U, (GRID_ROWS - height) / 2U, brightness);
}

template <typename T>
    requires std::is_unsigned_v<T>
void BitmapHandler<T>::draw(uint8_t x, uint8_t y, uint8_t brightness) const
{
    uint8_t msbMax{0U}; // NOLINT(misc-const-correctness)
    for (T bitset : bitmap)
    {
        uint8_t msb{0U};
        while (bitset >>= 1U)
        {
            ++msb;
        }
        if (msb > msbMax)
        {
            msbMax = msb;
        }
    }
    for (T _x{0U}; _x <= msbMax; ++_x)
    {
        for (uint8_t _y{0U}; _y < height; ++_y)
        {
            if ((bitmap[_y] >> (msbMax - _x)) & 0b1U)
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
