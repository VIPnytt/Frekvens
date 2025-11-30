#include "config/constants.h"

#if MODE_RING

#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h"
#include "modes/RingMode.h"
#include "services/DisplayService.h"

void RingMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Microphone->isTriggered())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();

        Display.clearFrame();
        BitmapHandler(ring[index]).draw();
        direction ? index++ : index--;
        if (index <= 0 || index >= ring.size() - 1)
        {
            direction = !direction;
        }
    }
}

#endif // MODE_RING
