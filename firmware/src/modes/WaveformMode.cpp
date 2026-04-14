#if MODE_WAVEFORM

#include "modes/WaveformMode.h"

#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

void WaveformMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > (1UL << 9U) && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis > (1UL << 9U))
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();

        Display.clearFrame();
        BitmapHandler(wave[random(wave.size())]).draw();
    }
}

#endif // MODE_WAVEFORM
