#include "handlers/ClockHandler.h"

#include "handlers/TextHandler.h"    // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)

void ClockHandler::handle()
{
    if (getLocalTime(&local) && (minute != local.tm_min || hour != local.tm_hour))
    {
        hour = local.tm_hour;
        minute = local.tm_min;
#if CLOCK_12H
        const int hour{(local.tm_hour + 11) % 12 + 1};
#endif // CLOCK_12H
        draw(digits[hour / 10], (GRID_COLUMNS / 2U) - 8U, 0U);
        draw(digits[hour % 10], (GRID_COLUMNS / 2U) - 4U, 0U);
        draw(digits[minute / 10], (GRID_COLUMNS / 2U) + 1U, 0U);
        draw(digits[minute % 10], (GRID_COLUMNS / 2U) + 5U, 0U);
    }
}

/**
 * @brief Renders a 3×5 bitmap at the specified display position.
 *
 * Set bits illuminate pixels. When opaque rendering is enabled, unset bits clear
 * the corresponding pixels.
 *
 * @param bitmap 15-bit bitmap containing the glyph pattern.
 * @param x Horizontal display position.
 * @param y Vertical display position.
 */
void ClockHandler::draw(uint16_t bitmap, uint8_t x, uint8_t y) const
{
    for (uint8_t _x{0U}; _x < 3U; ++_x)
    {
        for (uint8_t _y{0U}; _y < 5U; ++_y)
        {
            if (((bitmap >> (14U - ((3U * _y) + _x))) & 0b1U) != 0U)
            {
                Display.setPixel(static_cast<uint8_t>(x + _x), static_cast<uint8_t>(y + _y), UINT8_MAX);
            }
            else if (opaque)
            {
                Display.setPixel(static_cast<uint8_t>(x + _x), static_cast<uint8_t>(y + _y), 0U);
            }
        }
    }
}

/**
 * @brief Clears the first five rows of the display.
 */
void ClockHandler::clear() const { Display.fillRows(0U, 5U, 0U); }
