#if MODE_SMOOTHWAVEFORM

#include "modes/SmoothWaveformMode.h"

#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

void SmoothWaveformMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > (1UL << 9U) && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis > (1UL << 9U))
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();

        Display.clearFrame();
        BitmapHandler(smooth[random(smooth.size())]).draw();
    }
}

#endif // MODE_SMOOTHWAVEFORM
