#include "config/constants.h"

#if MODE_BLINDS

#include "extensions/MicrophoneExtension.h"
#include "modes/BlindsMode.h"
#include "services/DisplayService.h"

void BlindsMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > UINT8_MAX && Microphone->isTriggered())
#else
    if (millis() - lastMillis > UINT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();
        Display.clearFrame();
        for (uint8_t y = modulo / 2; y < GRID_COLUMNS; y += modulo)
        {
            for (uint8_t x = 0; x < GRID_COLUMNS; ++x)
            {
                Display.setPixel(x, y);
            }
        }
        if (modulo <= 1 || modulo >= moduloMax)
        {
            direction = !direction;
        }
        direction ? ++modulo : --modulo;
    }
}

#endif // MODE_BLINDS
