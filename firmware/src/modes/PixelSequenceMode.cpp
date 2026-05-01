#if MODE_PIXELSEQUENCE

#include "modes/PixelSequenceMode.h"

#include "extensions/MicrophoneExtension.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

void PixelSequenceMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; ++i)
        {
            if (pixels[i] == idx)
            {
                Display.setPixel(i % GRID_COLUMNS, i / GRID_COLUMNS, lit ? UINT8_MAX : 0);
                break;
            }
        }
        ++idx;
        if (idx >= GRID_COLUMNS * GRID_ROWS)
        {
            idx = 0;
            lit = !lit;
        }
        lastMillis = millis();
    }
}

#endif // MODE_PIXELSEQUENCE
