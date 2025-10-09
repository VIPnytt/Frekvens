#include "config/constants.h"

#if MODE_BLINDS

#include "extensions/MicrophoneExtension.h"
#include "modes/BlindsMode.h"
#include "services/DisplayService.h"

void BlindsMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > 160 && Microphone->play())
#else
    if (millis() - lastMillis > 160)
#endif
    {
        lastMillis = millis();
        Display.clear();
        for (uint8_t y = modulo / 2; y < COLUMNS; y += modulo)
        {
            for (uint8_t x = 0; x < COLUMNS; ++x)
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
