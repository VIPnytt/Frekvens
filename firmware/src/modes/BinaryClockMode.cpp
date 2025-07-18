#include "modes/BinaryClockMode.h"
#include "services/DisplayService.h"

void BinaryClockMode::wake()
{
    if (getLocalTime(&local))
    {
        pending = true;
        Display.clear();
    }
}

void BinaryClockMode::handle()
{
    if (getLocalTime(&local))
    {
        if (sec != local.tm_sec || pending)
        {
            sec = local.tm_sec;
            draw(11, sec);
        }
        if (min != local.tm_min || pending)
        {
            min = local.tm_min;
            draw(6, min);
        }
        if (hour != local.tm_hour || pending)
        {
            hour = local.tm_hour;
            draw(1, hour);
        }
        pending = false;
    }
}

void BinaryClockMode::draw(uint8_t y, uint8_t value)
{
    for (uint8_t i = 0; i < 6; ++i)
    {
        uint8_t x = 12 - (i * 2);
        Display.drawRectangle(x, y, x + 1, y + 3, true, value & (1U << i) ? UINT8_MAX : 0);
    }
}
