#if MODE_LINES

#include "modes/LinesMode.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "extensions/MicrophoneExtension.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

void LinesMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();
        for (uint8_t offset = 0; offset < GRID_COLUMNS + 2; offset += 4)
        {
            if (x + offset < GRID_COLUMNS)
            {
                for (uint8_t y = 0; y < GRID_ROWS; ++y)
                {
                    Display.setPixel(x + offset, y);
                }
            }
            if (x + offset - 2 >= 0 && x + offset - 2 < GRID_COLUMNS)
            {
                for (uint8_t y = 0; y < GRID_ROWS; ++y)
                {
                    Display.setPixel(x + offset - 2, y, 0);
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
