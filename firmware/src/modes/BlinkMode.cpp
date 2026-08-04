#if MODE_BLINK

#include "modes/BlinkMode.h"

#include "extensions/MicrophoneExtension.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

void BlinkMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > (1UL << 10U) && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis > (1UL << 10U))
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();
        Display.fillFrame(lit ? UINT8_MAX : 0U);
        lit = !lit;
    }
}

#endif // MODE_BLINK
