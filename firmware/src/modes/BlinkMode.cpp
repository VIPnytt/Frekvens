#if MODE_BLINK

#include "modes/BlinkMode.h"

#include "extensions/MicrophoneExtension.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

/**
 * @brief Advances the blink state when the update interval has elapsed.
 *
 * When microphone support is enabled, the display updates only when the
 * microphone is triggered. Each update alternates between maximum and zero
 * brightness.
 */
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
