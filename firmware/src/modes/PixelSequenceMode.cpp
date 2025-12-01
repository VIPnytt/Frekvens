#if MODE_PIXELSEQUENCE

#include "extensions/MicrophoneExtension.h"
#include "modes/PixelSequenceMode.h"
#include "services/DisplayService.h"

void PixelSequenceMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Microphone->isTriggered())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; ++i)
        {
            if (pixelOrder[i] == address)
            {
                Display.setPixel(i % GRID_COLUMNS, i / GRID_ROWS, lit ? UINT8_MAX : 0);
                break;
            }
        }
        ++address;
        if (address >= GRID_COLUMNS * GRID_ROWS)
        {
            address = 0;
            lit = !lit;
        }
        lastMillis = millis();
    }
}

#endif // MODE_PIXELSEQUENCE
