#include "config/constants.h"

#if MODE_LINES

#include "extensions/MicrophoneExtension.h"
#include "modes/LinesMode.h"
#include "services/DisplayService.h"

void LinesMode::handle()
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
            for (uint8_t n = 0; n - 2 < COLUMNS; n += 4)
            {
                if (x + n < COLUMNS)
                {
                    Display.setPixel(x + n, y);
                }
                if (x + n - 2 >= 0 && x + n - 2 < COLUMNS)
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
