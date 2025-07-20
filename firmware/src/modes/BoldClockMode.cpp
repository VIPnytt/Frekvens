#include "config/constants.h"
#include "fonts/MediumBoldFont.h"
#include "handlers/TextHandler.h"
#include "modes/BoldClockMode.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void BoldClockMode::handle()
{
    if (getLocalTime(&local))
    {
        if (minute != local.tm_min || hour != local.tm_hour)
        {
            hour = local.tm_hour;
            minute = local.tm_min;
            Display.clear();
            TextHandler h1 = TextHandler((String)(hour / 10), FontMediumBold);
            h1.draw(COLUMNS / 2 - 2 - h1.getWidth(), 0);
            TextHandler h2 = TextHandler((String)(hour % 10), FontMediumBold);
            h2.draw(COLUMNS / 2 + 2, 0);
            TextHandler m1 = TextHandler((String)(minute / 10), FontMediumBold);
            m1.draw(COLUMNS / 2 - 2 - m1.getWidth(), ROWS - m1.getHeight());
            TextHandler m2 = TextHandler((String)(minute % 10), FontMediumBold);
            m2.draw(COLUMNS / 2 + 2, ROWS - m2.getHeight());
        }
    }
}
