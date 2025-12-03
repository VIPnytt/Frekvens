#if MODE_LINES

#include "config/constants.h"
#include "extensions/MicrophoneExtension.h"
#include "modes/LinesMode.h"
#include "services/DisplayService.h"

void LinesMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Microphone->isTriggered())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();
        for (uint8_t y = 0; y < GRID_ROWS; ++y)
        {
            for (uint8_t n = 0; n < GRID_COLUMNS + 2; n += 4)
            {
                if (x + n < GRID_COLUMNS)
                {
                    Display.setPixel(x + n, y);
                }
                if (x + n - 2 >= 0 && x + n - 2 < GRID_COLUMNS)
                {
                    Display.setPixel(x + n - 2, y, 0);
                }
            }
        }
        ++x;
        if (x >= 4)
        {
            x = 0;
        }
    }
}

#endif // MODE_LINES
