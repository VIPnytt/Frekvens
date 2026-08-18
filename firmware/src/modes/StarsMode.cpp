#if MODE_STARS

#include "modes/StarsMode.h"

#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS * GRID_ROWS >= (0b1U << 4U),
              __STRING(MODE_STARS) " is not compatible with this device's display size.");

/**
 * @brief Initializes the star animation mode by clearing the display.
 */
void StarsMode::begin() { Display.fillFrame(0U); }

/**
     * @brief Advances the star animation based on each star's timing and brightness.
     *
     * Dimmed stars wait for a microphone trigger when microphone support is enabled;
     * otherwise, they reappear at an unoccupied pixel and begin brightening.
     */
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
                    star.idx = static_cast<size_t>(random(GRID_COLUMNS * GRID_ROWS));
                } while (Display.getPixel(star.idx) != 0U);
                ++star.brightness;
                star.direction = true;
                star.delay = static_cast<uint8_t>(random(0b1U << 4U));
            }
            Display.setPixel(star.idx, star.brightness);
            star.lastMillis = millis();
        }
    }
}

#endif // MODE_STARS
