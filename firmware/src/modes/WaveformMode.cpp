#include "config/constants.h"

#if MODE_WAVEFORM

#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h"
#include "modes/WaveformMode.h"
#include "services/DisplayService.h"

void WaveformMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > (1 << 9) && Microphone->isPlay())
#else
    if (millis() - lastMillis > (1 << 9))
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();

        Display.clearFrame();
        BitmapHandler bitmap = BitmapHandler(wave[random(wave.size())]);
        bitmap.draw((GRID_COLUMNS - bitmap.getWidth()) / 2, (GRID_ROWS - bitmap.getHeight()) / 2);
    }
}

#endif // MODE_WAVEFORM
