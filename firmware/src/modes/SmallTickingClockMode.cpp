#if MODE_SMALLTICKINGCLOCK

#include "config/constants.h"
#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "modes/SmallTickingClockMode.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void SmallTickingClockMode::begin()
{
    pending = true;
}

void SmallTickingClockMode::handle()
{
    if (getLocalTime(&local))
    {
        if (minute != local.tm_min || hour != local.tm_hour || pending)
        {
            hour = local.tm_hour;
            minute = local.tm_min;
            Display.clearFrame();
            TextHandler(std::to_string(hour / 10), FontMini).draw(GRID_COLUMNS / 2 - 4, GRID_ROWS / 2 - 6);
            TextHandler(std::to_string(hour % 10), FontMini).draw(GRID_COLUMNS / 2 + 1, GRID_ROWS / 2 - 6);
            TextHandler(std::to_string(minute / 10), FontMini).draw(GRID_COLUMNS / 2 - 4, GRID_ROWS / 2);
            TextHandler(std::to_string(minute % 10), FontMini).draw(GRID_COLUMNS / 2 + 1, GRID_ROWS / 2);
            pending = false;
        }
        if (second != local.tm_sec)
        {
            second = local.tm_sec;
            if (second < 8)
            {
                Display.setPixel(GRID_COLUMNS / 2 - 1 + second, GRID_ROWS / 2 - 8, 0);
                Display.setPixel(GRID_COLUMNS / 2 + second, GRID_ROWS / 2 - 8, INT8_MAX);
            }
            else if (second < 8 + 15)
            {
                Display.setPixel(GRID_COLUMNS / 2 + 7, GRID_ROWS / 2 - 16 + second, 0);
                Display.setPixel(GRID_COLUMNS / 2 + 7, GRID_ROWS / 2 - 15 + second, INT8_MAX);
            }
            else if (second < 8 + 15 * 2)
            {
                Display.setPixel(GRID_COLUMNS / 2 + 30 - second, GRID_ROWS / 2 + 7, 0);
                Display.setPixel(GRID_COLUMNS / 2 + 29 - second, GRID_ROWS / 2 + 7, INT8_MAX);
            }
            else if (second < 8 + 15 * 3)
            {
                Display.setPixel(GRID_COLUMNS / 2 - 8, GRID_ROWS / 2 + 45 - second, 0);
                Display.setPixel(GRID_COLUMNS / 2 - 8, GRID_ROWS / 2 + 44 - second, INT8_MAX);
            }
            else
            {
                Display.setPixel(GRID_COLUMNS / 2 - 61 + second, GRID_ROWS / 2 - 8, 0);
                Display.setPixel(GRID_COLUMNS / 2 - 60 + second, GRID_ROWS / 2 - 8, INT8_MAX);
            }
        }
    }
}

#endif // MODE_SMALLTICKINGCLOCK
