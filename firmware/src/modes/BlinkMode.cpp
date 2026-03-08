#if MODE_BLINK

#include "modes/BlinkMode.h"

#include "extensions/MicrophoneExtension.h"
#include "services/DisplayService.h"

void BlinkMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > (1UL << 10U) && Microphone->isTriggered())
#else
    if (millis() - lastMillis > (1UL << 10U))
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();
        Display.clearFrame(Display.getPixel(0, 0) == 0 ? UINT8_MAX : 0);
    }
}

#endif // MODE_BLINK
