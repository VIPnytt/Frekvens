#if MODE_STARS

#include "modes/StarsMode.h"

#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS * GRID_ROWS >= (0b1U << 4U),
              __STRING(MODE_STARS) " is not compatible with this device's display size.");

void StarsMode::begin() { Display.clearFrame(); }

void StarsMode::handle()
{
    for (Star &star : stars)
    {
        if (millis() - star.lastMillis > star.delay)
        {
            if (star.brightness == UINT8_MAX)
            {
                star.direction = false;
                --star.brightness;
            }
            else if (star.brightness != 0U)
            {
                star.direction ? ++star.brightness : --star.brightness;
            }
#if EXTENSION_MICROPHONE
            else if (!Extensions.Microphone().isTriggered())
            {
                star.lastMillis = millis();
                break;
            }
#endif // EXTENSION_MICROPHONE
            else
            {
                do
                {
                    star.x = random(GRID_COLUMNS);
                    star.y = random(GRID_ROWS);
                } while (Display.getPixel(star.x, star.y) != 0U);
                ++star.brightness;
                star.direction = true;
                star.delay = random(0b1U << 4U);
            }
            Display.setPixel(star.x, star.y, star.brightness);
            star.lastMillis = millis();
        }
    }
}

#endif // MODE_STARS
