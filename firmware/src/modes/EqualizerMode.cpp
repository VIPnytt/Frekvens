#include "config/constants.h"
#include "extensions/MicrophoneExtension.h"
#include "modes/EqualizerMode.h"
#include "services/DisplayService.h"

void EqualizerMode::wake()
{
    Display.clear();
}

void EqualizerMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > interval && Microphone->play())
#else
    if (millis() - lastMillis > interval)
#endif
    {
        lastMillis = millis();
        interval = random(100, 900);

        const uint8_t
            h = random(1, ROWS),
            x = random(COLUMNS / (width + 1)) * (width + 1);

        Display.drawRectangle(x, ROWS - 1 - h, x + width - 1, ROWS - 1, true, UINT8_MAX / ROWS * (h + 1));
        if (h < ROWS - 1)
        {
            Display.drawRectangle(x, 0, x + width - 1, ROWS - 1 - h, true, 0);
        }
    }
}
