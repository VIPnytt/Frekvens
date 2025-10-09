#include "config/constants.h"

#if MODE_PIXELSEQUENCE

#include "extensions/MicrophoneExtension.h"
#include "modes/PixelSequenceMode.h"
#include "services/DisplayService.h"

void PixelSequenceMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Microphone->play())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif
    {
        for (uint16_t i = 0; i < COLUMNS * ROWS; ++i)
        {
            if (pixelOrder[i] == address)
            {
                Display.setPixel(i % COLUMNS, i / ROWS, brightness);
                break;
            }
        }
        ++address;
        if (address >= COLUMNS * ROWS)
        {
            address = 0;
            brightness = brightness ? 0 : UINT8_MAX;
        }
        lastMillis = millis();
    }
}

#endif // MODE_PIXELSEQUENCE
