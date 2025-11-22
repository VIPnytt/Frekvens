#include "config/constants.h"

#if MODE_SCAN

#include "extensions/MicrophoneExtension.h"
#include "modes/ScanMode.h"
#include "services/DisplayService.h"

void ScanMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Microphone->isPlay())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();
        for (uint8_t y = 0; y < GRID_ROWS; ++y)
        {
            if (x < GRID_COLUMNS)
            {
                Display.setPixel(x, y);
            }
            if (x - 2 >= 0)
            {
                Display.setPixel(x - 2, y, 0);
            }
        }
        ++x;
        if (x - 2 >= GRID_COLUMNS)
        {
            x = 0;
        }
    }
}

#endif // MODE_SCAN
