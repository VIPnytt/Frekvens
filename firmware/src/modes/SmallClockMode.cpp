#if MODE_SMALLCLOCK

#include "config/constants.h"
#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "modes/SmallClockMode.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void SmallClockMode::begin()
{
    pending = true;
}

void SmallClockMode::handle()
{
    if (getLocalTime(&local) && (minute != local.tm_min || hour != local.tm_hour || pending))
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
}

#endif // MODE_SMALLCLOCK
