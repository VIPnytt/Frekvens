#if MODE_BINARYCLOCK

#include "modes/BinaryClockMode.h"

#include "config/constants.h"
#include "services/DisplayService.h"

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
            draw((GRID_ROWS / 2) + 3, second);
        }
        if (minute != local.tm_min || pending)
        {
            minute = local.tm_min;
            draw((GRID_ROWS / 2) - 2, minute);
        }
        if (hour != local.tm_hour || pending)
        {
            hour = local.tm_hour;
            draw((GRID_ROWS / 2) - 7, hour);
            pending = false;
        }
    }
}

void BinaryClockMode::draw(uint8_t y, uint8_t value)
{
    for (uint8_t i = 0; i < 6; ++i)
    {
        const uint8_t x = (GRID_COLUMNS / 2) + 4 - (i * 2);
        Display.drawRectangle(x, y, x + 1, y + 3, true, (value & (1 << i)) != 0 ? UINT8_MAX : 0);
    }
}

#endif // MODE_BINARYCLOCK
