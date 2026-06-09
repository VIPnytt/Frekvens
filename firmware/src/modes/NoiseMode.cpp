#if MODE_NOISE

#include "modes/NoiseMode.h"

#include "services/DisplayService.h"    // NOLINT(misc-include-cleaner)
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS * GRID_ROWS >= 9U,
              __STRING(MODE_NOISE) " is not compatible with this device's display size.");

void NoiseMode::handle()
{
    for (Dot &dot : dots)
    {
        if (millis() - dot.lastMillis > dot.delay)
        {
#if EXTENSION_MICROPHONE
            if (!Extensions.Microphone().isTriggered())
            {
                dot.lastMillis = millis();
                break;
            }
#endif // EXTENSION_MICROPHONE
            Display.setPixel(dot.x, dot.y, 0U);
            dot.x = random(GRID_COLUMNS);
            dot.y = random(GRID_ROWS);
            dot.delay = random(0b1U << 6U, INT8_MAX);
            Display.setPixel(dot.x, dot.y);
            dot.lastMillis = millis();
        }
    }
}

#endif // MODE_NOISE
