#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "modes/SmallClockMode.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void SmallClockMode::wake()
{
    pending = true;
}

void SmallClockMode::handle()
{
    if (getLocalTime(&local) && (minute != local.tm_min || hour != local.tm_hour || pending))
    {
        hour = local.tm_hour;
        minute = local.tm_min;
        Display.clear();
        TextHandler((String)(hour / 10), FontMini).draw(4, 2);
        TextHandler((String)(hour % 10), FontMini).draw(9, 2);
        TextHandler((String)(minute / 10), FontMini).draw(4, 8);
        TextHandler((String)(minute % 10), FontMini).draw(9, 8);
        pending = false;
    }
}
