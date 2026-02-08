#if MODE_STARS

#include "modes/StarsMode.h"

#include "services/DisplayService.h"

void StarsMode::begin() { Display.clearFrame(); }

void StarsMode::handle()
{
    for (Star &star : stars)
    {
        if (millis() - star.lastMillis > star.delay)
        {
            if (star.brightness >= UINT8_MAX)
            {
                --star.brightness;
                star.direction = false;
            }
            else if (star.brightness > 0)
            {
                star.brightness += star.direction ? 1 : -1;
            }
            else
            {
                do
                {
                    star.x = random(GRID_COLUMNS);
                    star.y = random(GRID_ROWS);
                } while (Display.getPixel(star.x, star.y) > 0);
                ++star.brightness;
                star.direction = true;
                star.delay = random(1 << 4);
            }
            Display.setPixel(star.x, star.y, star.brightness);
            star.lastMillis = millis();
        }
    }
}

#endif // MODE_STARS
