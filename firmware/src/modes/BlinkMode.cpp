#include "config/constants.h"

#if MODE_BLINK

#include "extensions/MicrophoneExtension.h"
#include "modes/BlinkMode.h"
#include "services/DisplayService.h"

void BlinkMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > (1 << 10) && Microphone->isPlay())
#else
    if (millis() - lastMillis > (1 << 10))
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();
        Display.clearFrame(Display.getPixel(0, 0) ? 0 : UINT8_MAX);
    }
}

#endif // MODE_BLINK
