#if MODE_RING

#include "modes/RingMode.h"

#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

/**
 * @brief Updates and displays the ring animation when an update is due.
 *
 * With microphone support enabled, an update also requires the microphone to
 * be triggered. Each update redraws the current ring frame and reverses the
 * animation direction at either end of the ring sequence.
 */
void RingMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();

        Display.fillFrame(0U);
        BitmapHandler(ring[index]).draw();
        direction ? index++ : index--;
        if (index <= 0 || index >= ring.size() - 1)
        {
            direction = !direction;
        }
    }
}

#endif // MODE_RING
