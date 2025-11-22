#include "config/constants.h"

#if MODE_RING

#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h"
#include "modes/RingMode.h"
#include "services/DisplayService.h"

void RingMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Microphone->isPlay())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();

        Display.clearFrame();
        BitmapHandler bitmap = BitmapHandler(ring[frame]);
        bitmap.draw((GRID_COLUMNS - bitmap.getWidth()) / 2, (GRID_ROWS - bitmap.getHeight()) / 2);
        direction ? frame++ : frame--;
        if (frame <= 0 || frame >= ring.size() - 1)
        {
            direction = !direction;
        }
    }
}

#endif // MODE_RING
