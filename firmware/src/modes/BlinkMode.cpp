#if MODE_BLINK

#include "modes/BlinkMode.h"

#include "extensions/MicrophoneExtension.h"
#include "services/DisplayService.h"

void BlinkMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > (1 << 10) && Microphone->isTriggered())
#else
    if (millis() - lastMillis > (1 << 10))
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();
        Display.clearFrame(Display.getPixel(0, 0) == 0 ? UINT8_MAX : 0);
    }
}

#endif // MODE_BLINK
