#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "modes/SmallClockMode.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void SmallClockMode::handle()
{
    if (getLocalTime(&local))
    {
        if (min != local.tm_min || hour != local.tm_hour)
        {
            hour = local.tm_hour;
            min = local.tm_min;
            Display.clear();
            TextHandler((String)(hour / 10), FontMini).draw(4, 2);
            TextHandler((String)(hour % 10), FontMini).draw(9, 2);
            TextHandler((String)(min / 10), FontMini).draw(4, 8);
            TextHandler((String)(min % 10), FontMini).draw(9, 8);
        }
    }
}
