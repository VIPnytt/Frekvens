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
        const uint8_t hour{static_cast<uint8_t>((local.tm_hour + 11) % 12 + 1)};
#endif // CLOCK_12H
        draw(digits[hour / 10], GRID_COLUMNS / 2U - 8U, 0U);
        draw(digits[hour % 10], GRID_COLUMNS / 2U - 4U, 0U);
        draw(digits[minute / 10], GRID_COLUMNS / 2U + 1U, 0U);
        draw(digits[minute % 10], GRID_COLUMNS / 2U + 5U, 0U);
    }
}

void ClockHandler::draw(const std::array<uint8_t, 5U> &bitmap, uint8_t x, uint8_t y) const
{
    for (uint8_t _x{x}; _x < x + 3U; ++_x)
    {
        for (uint8_t _y{y}; _y < y + 5U; ++_y)
        {
            if ((bitmap[_y - y] >> (x + 2U - _x)) & 1U)
            {
                Display.setPixel(_x, _y);
            }
            else if (opaque)
            {
                Display.setPixel(_x, _y, 0U);
            }
        }
    }
}

void ClockHandler::clear()
{
    for (uint8_t y{0U}; y < 5U; ++y)
    {
        Display.setPixel(GRID_COLUMNS / 2U - 8U, y, 0U);
        Display.setPixel(GRID_COLUMNS / 2U - 7U, y, 0U);
        Display.setPixel(GRID_COLUMNS / 2U - 6U, y, 0U);
        Display.setPixel(GRID_COLUMNS / 2U - 4U, y, 0U);
        Display.setPixel(GRID_COLUMNS / 2U - 3U, y, 0U);
        Display.setPixel(GRID_COLUMNS / 2U - 2U, y, 0U);
        Display.setPixel(GRID_COLUMNS / 2U + 1U, y, 0U);
        Display.setPixel(GRID_COLUMNS / 2U + 2U, y, 0U);
        Display.setPixel(GRID_COLUMNS / 2U + 3U, y, 0U);
        Display.setPixel(GRID_COLUMNS / 2U + 5U, y, 0U);
        Display.setPixel(GRID_COLUMNS / 2U + 6U, y, 0U);
        Display.setPixel(GRID_COLUMNS / 2U + 7U, y, 0U);
    }
}
