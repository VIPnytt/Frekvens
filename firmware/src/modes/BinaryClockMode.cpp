#include "config/constants.h"

#if MODE_BINARYCLOCK

#include "modes/BinaryClockMode.h"
#include "services/DisplayService.h"

void BinaryClockMode::wake()
{
    pending = true;
}

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
            draw(11, second);
        }
        if (minute != local.tm_min || pending)
        {
            minute = local.tm_min;
            draw(6, minute);
        }
        if (hour != local.tm_hour || pending)
        {
            hour = local.tm_hour;
            draw(1, hour);
            pending = false;
        }
    }
}

void BinaryClockMode::draw(uint8_t y, uint8_t value)
{
    for (uint8_t i = 0; i < 6; ++i)
    {
        const uint8_t x = 12 - i * 2;
        Display.drawRectangle(x, y, x + 1, y + 3, true, value & (1 << i) ? UINT8_MAX : 0);
    }
}

#endif // MODE_BINARYCLOCK
