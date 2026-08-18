#if MODE_SCAN

#include "modes/ScanMode.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

static_assert(GRID_COLUMNS >= 3U, __STRING(MODE_SCAN) " is not compatible with this device's display size.");

/**
 * @brief Advances the scanning highlight across the display.
 *
 * When microphone support is enabled, advancement also requires a microphone
 * trigger. The highlight wraps after passing the display columns and clears
 * the column two positions behind it.
 */
void ScanMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();
        if (x < GRID_COLUMNS)
        {
            Display.fillColumn(x, UINT8_MAX);
        }
        if (x >= 2U)
        {
            Display.fillColumn(x - 2U, 0U);
        }
        ++x;
        if (x >= GRID_COLUMNS + 2U)
        {
            x = 0U;
        }
    }
}

#endif // MODE_SCAN
