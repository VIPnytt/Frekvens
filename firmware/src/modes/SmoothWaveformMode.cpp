#if MODE_SMOOTHWAVEFORM

#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h"
#include "modes/SmoothWaveformMode.h"
#include "services/DisplayService.h"

void SmoothWaveformMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > (1 << 9) && Microphone->isTriggered())
#else
    if (millis() - lastMillis > (1 << 9))
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();

        Display.clearFrame();
        BitmapHandler(smooth[random(smooth.size())]).draw();
    }
}

#endif // MODE_SMOOTHWAVEFORM
