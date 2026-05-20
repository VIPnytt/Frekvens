#if MODE_BINARYCLOCK

#include "modes/BinaryClockMode.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"

static_assert(GRID_COLUMNS >= 12U, __STRING(MODE_BINARYCLOCK) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 14U, __STRING(MODE_BINARYCLOCK) " is not compatible with this device's display size.");

void BinaryClockMode::begin() { pending = true; }

void BinaryClockMode::handle()
{
    if (getLocalTime(&local))
    {
        if (pending)
        {
            Display.clearFrame();
        }
        if (second != local.tm_sec || pending)
        {
            second = local.tm_sec;
            draw((GRID_ROWS / 2U) + 3U, second);
        }
        if (minute != local.tm_min || pending)
        {
            minute = local.tm_min;
            draw((GRID_ROWS / 2U) - 2U, minute);
        }
        if (hour != local.tm_hour || pending)
        {
            hour = local.tm_hour;
            draw((GRID_ROWS / 2U) - 7U, hour);
            pending = false;
        }
    }
}

void BinaryClockMode::draw(uint8_t y, uint8_t value)
{
    for (uint8_t i{0U}; i < 6U; ++i)
    {
        const uint8_t x{static_cast<uint8_t>((GRID_COLUMNS / 2U) + 4U - (i * 2U))};
        Display.drawRectangle(x, y, x + 1U, y + 3U, true, (value & (1U << i)) != 0U ? UINT8_MAX : 0U);
    }
}

#endif // MODE_BINARYCLOCK
