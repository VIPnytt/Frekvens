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
        if (modulo == 1U)
        {
            Display.fillFrame(UINT8_MAX);
            direction = true;
            ++modulo;
        }
        else
        {
            Display.fillFrame(0U);
            for (uint8_t y{static_cast<uint8_t>(modulo / 2U)}; y < GRID_ROWS; y += modulo)
            {
                Display.fillRow(y, UINT8_MAX);
            }
            if (modulo == moduloMax)
            {
                direction = !direction;
                --modulo;
            }
            else
            {
                direction ? ++modulo : --modulo;
            }
        }
    }
}

#endif // MODE_BLINDS
