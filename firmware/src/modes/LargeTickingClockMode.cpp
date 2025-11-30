#include "config/constants.h"

#if MODE_LARGETICKINGCLOCK

#include "fonts/MediumFont.h"
#include "handlers/TextHandler.h"
#include "modes/LargeTickingClockMode.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void LargeTickingClockMode::begin()
{
    pending = true;
}

void LargeTickingClockMode::handle()
{
    if (getLocalTime(&local))
    {
        if (minute != local.tm_min || hour != local.tm_hour || pending)
        {
            hour = local.tm_hour;
            minute = local.tm_min;
            Display.clearFrame();
            TextHandler h1 = TextHandler(std::to_string(hour / 10), FontMedium);
            h1.draw(GRID_COLUMNS / 2 - 1 - h1.getWidth(), 0);
            TextHandler h2 = TextHandler(std::to_string(hour % 10), FontMedium);
            h2.draw(GRID_COLUMNS - h2.getWidth(), 0);
            TextHandler m1 = TextHandler(std::to_string(minute / 10), FontMedium);
            m1.draw(GRID_COLUMNS / 2 - 1 - m1.getWidth(), GRID_ROWS - m1.getHeight());
            TextHandler m2 = TextHandler(std::to_string(minute % 10), FontMedium);
            m2.draw(GRID_COLUMNS - m2.getWidth(), GRID_ROWS - m2.getHeight());
            pending = false;
        }
        if (secound != local.tm_sec)
        {
            Display.setPixel(GRID_COLUMNS / 2 - 8 + (secound + 2) / 4, secound % 2 ? GRID_ROWS / 2 : GRID_ROWS / 2 - 1, 0);
            secound = local.tm_sec;
            Display.setPixel(GRID_COLUMNS / 2 - 8 + (secound + 2) / 4, secound % 2 ? GRID_ROWS / 2 : GRID_ROWS / 2 - 1, INT8_MAX);
        }
    }
}

#endif // MODE_LARGETICKINGCLOCK
