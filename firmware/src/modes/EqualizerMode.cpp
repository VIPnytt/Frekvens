#include "config/constants.h"

#if MODE_EQUALIZER

#include "extensions/MicrophoneExtension.h"
#include "modes/EqualizerMode.h"
#include "services/DisplayService.h"

void EqualizerMode::wake()
{
    Display.clearFrame();
}

void EqualizerMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > interval && Microphone->isPlay())
#else
    if (millis() - lastMillis > interval)
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();
        interval = random(INT8_MAX, 1 << 9);

        const uint8_t
            h = random(1, GRID_ROWS),
            x = random(GRID_COLUMNS / (width + 1)) * (width + 1);

        Display.drawRectangle(x, GRID_ROWS - 1 - h, x + width - 1, GRID_ROWS - 1, true, UINT8_MAX / GRID_ROWS * (h + 1));
        if (h < GRID_ROWS - 1)
        {
            Display.drawRectangle(x, 0, x + width - 1, GRID_ROWS - 1 - h, true, 0);
        }
    }
}

#endif // MODE_EQUALIZER
