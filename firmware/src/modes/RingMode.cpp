#include "config/constants.h"

#if MODE_RING

#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h"
#include "modes/RingMode.h"
#include "services/DisplayService.h"

void RingMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Microphone->play())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif
    {
        lastMillis = millis();

        Display.clear();
        BitmapHandler bitmap = BitmapHandler(ring[frame]);
        bitmap.draw((COLUMNS - bitmap.getWidth()) / 2, (ROWS - bitmap.getHeight()) / 2);
        direction ? frame++ : frame--;
        if (frame <= 0 || frame >= ring.size() - 1)
        {
            direction = !direction;
        }
    }
}

#endif // MODE_RING
