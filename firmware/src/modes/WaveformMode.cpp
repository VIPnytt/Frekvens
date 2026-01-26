#if MODE_WAVEFORM

#include "modes/WaveformMode.h"

#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h"
#include "services/DisplayService.h"

void WaveformMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > (1 << 9) && Microphone->isTriggered())
#else
    if (millis() - lastMillis > (1 << 9))
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();

        Display.clearFrame();
        BitmapHandler(wave[random(wave.size())]).draw();
    }
}

#endif // MODE_WAVEFORM
