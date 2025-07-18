#include "config/constants.h"
#include "extensions/MicrophoneExtension.h"
#include "modes/BlinkMode.h"
#include "services/DisplayService.h"

void BlinkMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > 1000 && Microphone->play())
#else
    if (millis() - lastMillis > 1000)
#endif
    {
        lastMillis = millis();
        Display.clear(Display.getPixel(0, 0) ? 0 : UINT8_MAX);
    }
}
