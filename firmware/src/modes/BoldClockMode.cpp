#if MODE_BOLDCLOCK

#include "config/constants.h"
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
        {
            TextHandler h1 = TextHandler(std::to_string(hour / 10), FontMediumBold);
            h1.draw(GRID_COLUMNS / 2 - 1 - (6 - h1.getWidth()) / 2 - h1.getWidth(), GRID_ROWS / 2 - 1 - (7 - h1.getHeight()) / 2 - h1.getHeight());
        }
        {
            TextHandler h2 = TextHandler(std::to_string(hour % 10), FontMediumBold);
            h2.draw(GRID_COLUMNS / 2 + 1 + (6 - h2.getWidth()) / 2, GRID_ROWS / 2 - 1 + (7 - h2.getHeight()) / 2 - h2.getHeight());
        }
        {
            TextHandler m1 = TextHandler(std::to_string(minute / 10), FontMediumBold);
            m1.draw(GRID_COLUMNS / 2 - 1 - (6 - m1.getWidth()) / 2 - m1.getWidth(), GRID_ROWS / 2 + 1 - (7 - m1.getHeight()) / 2);
        }
        {
            TextHandler m2 = TextHandler(std::to_string(minute % 10), FontMediumBold);
            m2.draw(GRID_COLUMNS / 2 + 1 + (6 - m2.getWidth()) / 2, GRID_ROWS / 2 + 1 + (7 - m2.getHeight()) / 2);
        }
        pending = false;
    }
}

#endif // MODE_BOLDCLOCK
