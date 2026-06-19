#if MODE_SCAN

#include "modes/ScanMode.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

static_assert(GRID_COLUMNS >= 3U, __STRING(MODE_SCAN) " is not compatible with this device's display size.");

void ScanMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();
        for (uint8_t y{0U}; y < GRID_ROWS; ++y)
        {
            if (column < GRID_COLUMNS)
            {
                Display.setPixel(column, y);
            }
            if (column >= 2U)
            {
                Display.setPixel(column - 2U, y, 0U);
            }
        }
        ++column;
        if (column >= GRID_COLUMNS + 2U)
        {
            column = 0U;
        }
    }
}

#endif // MODE_SCAN
