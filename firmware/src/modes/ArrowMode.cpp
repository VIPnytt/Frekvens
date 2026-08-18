#if MODE_ARROW

#include "modes/ArrowMode.h"

#include "config/constants.h"        // NOLINT(misc-include-cleaner)
#include "handlers/BitmapHandler.h"  // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)
#include "services/ExtensionsService.h"

static_assert(GRID_COLUMNS >= 15U, __STRING(MODE_ARROW) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 15U, __STRING(MODE_ARROW) " is not compatible with this device's display size.");

/**
 * @brief Advances and displays the arrow animation when its update interval has elapsed.
 *
 * When microphone support is enabled, an update also requires a microphone trigger.
 */
void ArrowMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();
        Display.fillFrame(0U);
        if (index < arrows0.size())
        {
            const BitmapHandler bitmap(arrows0[index]);
            bitmap.draw((GRID_COLUMNS - bitmap.getWidth()) / 2U, GRID_ROWS - bitmap.getHeight());
        }
        else
        {
            const BitmapHandler bitmap(arrows1[index - arrows0.size()]);
            bitmap.draw((GRID_COLUMNS - bitmap.getWidth()) / 2U, GRID_ROWS - bitmap.getHeight());
        }
        if (index == 0U)
        {
            direction = true;
        }
        else if (index == arrows0.size() + arrows1.size() - 1U)
        {
            direction = false;
        }
        direction ? ++index : --index;
    }
}

#endif // MODE_ARROW
