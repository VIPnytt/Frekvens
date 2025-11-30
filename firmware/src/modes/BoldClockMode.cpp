#include "config/constants.h"

#if MODE_BOLDCLOCK

#include "fonts/MediumBoldFont.h"
#include "handlers/TextHandler.h"
#include "modes/BoldClockMode.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void BoldClockMode::begin()
{
    pending = true;
}

void BoldClockMode::handle()
{
    if (getLocalTime(&local) && (minute != local.tm_min || hour != local.tm_hour || pending))
    {
        hour = local.tm_hour;
        minute = local.tm_min;
        Display.clearFrame();
        TextHandler h1 = TextHandler(std::to_string(hour / 10), FontMediumBold);
        h1.draw(GRID_COLUMNS / 2 - 2 - h1.getWidth(), 0);
        TextHandler h2 = TextHandler(std::to_string(hour % 10), FontMediumBold);
        h2.draw(GRID_COLUMNS / 2 + 2, 0);
        TextHandler m1 = TextHandler(std::to_string(minute / 10), FontMediumBold);
        m1.draw(GRID_COLUMNS / 2 - 2 - m1.getWidth(), GRID_ROWS - m1.getHeight());
        TextHandler m2 = TextHandler(std::to_string(minute % 10), FontMediumBold);
        m2.draw(GRID_COLUMNS / 2 + 2, GRID_ROWS - m2.getHeight());
        pending = false;
    }
}

#endif // MODE_BOLDCLOCK
