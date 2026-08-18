#if MODE_NOISE

#include "modes/NoiseMode.h"

#include "services/DisplayService.h"    // NOLINT(misc-include-cleaner)
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS * GRID_ROWS >= 9U,
              __STRING(MODE_NOISE) " is not compatible with this device's display size.");

/**
 * @brief Updates noise dots whose movement delays have elapsed.
 *
 * When microphone support is enabled, updates are deferred until the microphone
 * is triggered.
 */
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
            Display.setPixel(dot.idx, 0U);
            dot.idx = static_cast<size_t>(random(GRID_COLUMNS * GRID_ROWS));
            Display.setPixel(dot.idx, UINT8_MAX);
            dot.delay = static_cast<uint8_t>(random(0b1U << 6U, INT8_MAX));
            dot.lastMillis = millis();
        }
    }
}

#endif // MODE_NOISE
