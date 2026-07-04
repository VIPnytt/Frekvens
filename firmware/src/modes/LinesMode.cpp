#if MODE_LINES

#include "modes/LinesMode.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

static_assert(GRID_COLUMNS >= 4U, __STRING(MODE_LINES) " is not compatible with this device's display size.");

void LinesMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();
        for (uint8_t offset{0U}; offset < GRID_COLUMNS + 2U; offset += 4U)
        {
            if (x + offset < GRID_COLUMNS)
            {
                for (uint8_t y{0U}; y < GRID_ROWS; ++y)
                {
                    Display.setPixel(x + offset, y, UINT8_MAX);
                }
            }
            if (x + offset >= 2U && x + offset < GRID_COLUMNS + 2U)
            {
                for (uint8_t y{0U}; y < GRID_ROWS; ++y)
                {
                    Display.setPixel(x + offset - 2U, y, 0U);
                }
            }
        }
        ++x;
        if (x >= 4U)
        {
            x = 0U;
        }
    }
}

#endif // MODE_LINES
