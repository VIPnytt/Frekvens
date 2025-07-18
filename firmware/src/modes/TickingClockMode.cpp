#include "config/constants.h"
#include "fonts/MediumFont.h"
#include "handlers/TextHandler.h"
#include "modes/TickingClockMode.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void TickingClockMode::handle()
{
    if (getLocalTime(&local))
    {
        if (min != local.tm_min || hour != local.tm_hour)
        {
            hour = local.tm_hour;
            min = local.tm_min;
            Display.clear();
            TextHandler h1 = TextHandler((String)(hour / 10), FontMedium);
            h1.draw(COLUMNS / 2 - 1 - h1.getWidth(), 0);
            TextHandler h2 = TextHandler((String)(hour % 10), FontMedium);
            h2.draw(COLUMNS - h2.getWidth(), 0);
            TextHandler m1 = TextHandler((String)(min / 10), FontMedium);
            m1.draw(COLUMNS / 2 - 1 - m1.getWidth(), ROWS - m1.getHeight());
            TextHandler m2 = TextHandler((String)(min % 10), FontMedium);
            m2.draw(COLUMNS - m2.getWidth(), ROWS - m2.getHeight());
        }
        if (sec != local.tm_sec)
        {
            Display.setPixel(sec * COLUMNS / 60, (sec * COLUMNS / 30) % 2 ? 8 : 7, 0);
            sec = local.tm_sec;
            Display.setPixel(sec * COLUMNS / 60, (sec * COLUMNS / 30) % 2 ? 8 : 7);
        }
    }
}
