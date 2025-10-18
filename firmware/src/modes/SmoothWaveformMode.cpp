#include "config/constants.h"

#if MODE_SMOOTHWAVEFORM

#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h"
#include "modes/SmoothWaveformMode.h"
#include "services/DisplayService.h"

void SmoothWaveformMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > (1 << 9) && Microphone->play())
#else
    if (millis() - lastMillis > (1 << 9))
#endif
    {
        lastMillis = millis();

        Display.clear();
        BitmapHandler bitmap = BitmapHandler(smooth[random(smooth.size())]);
        bitmap.draw((COLUMNS - bitmap.getWidth()) / 2, (ROWS - bitmap.getHeight()) / 2);
    }
}

#endif // MODE_SMOOTHWAVEFORM
