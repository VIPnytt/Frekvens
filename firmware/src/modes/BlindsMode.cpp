#if MODE_BLINDS

#include "modes/BlindsMode.h"

#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

static_assert(GRID_ROWS >= 6U, __STRING(MODE_BLINDS) " is not compatible with this device's display size.");

void BlindsMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > UINT8_MAX && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis > UINT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();
        Display.clearFrame();
        for (uint8_t y{static_cast<uint8_t>(modulo / 2U)}; y < GRID_ROWS; y += modulo)
        {
            for (uint8_t x{0U}; x < GRID_COLUMNS; ++x)
            {
                Display.setPixel(x, y, UINT8_MAX);
            }
        }
        if (modulo == 1U || modulo == moduloMax)
        {
            direction = !direction;
        }
        direction ? ++modulo : --modulo;
    }
}

#endif // MODE_BLINDS
