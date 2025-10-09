#include "config/constants.h"

#if MODE_SCAN

#include "extensions/MicrophoneExtension.h"
#include "modes/ScanMode.h"
#include "services/DisplayService.h"

void ScanMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > 100 && Microphone->play())
#else
    if (millis() - lastMillis > 100)
#endif
    {
        lastMillis = millis();
        for (uint8_t y = 0; y < ROWS; ++y)
        {
            if (x < COLUMNS)
            {
                Display.setPixel(x, y);
            }
            if (x - 2 >= 0)
            {
                Display.setPixel(x - 2, y, 0);
            }
        }
        ++x;
        if (x - 2 >= COLUMNS)
        {
            x = 0;
        }
    }
}

#endif // MODE_SCAN
