#include "config/constants.h"
#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h"
#include "modes/SmoothWaveformMode.h"
#include "services/DisplayService.h"

void SmoothWaveformMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > 500 && Microphone->play())
#else
    if (millis() - lastMillis > 500)
#endif
    {
        lastMillis = millis();

        Display.clear();
        BitmapHandler bitmap = BitmapHandler(smooth[random(smooth.size())]);
        bitmap.draw((COLUMNS - bitmap.getWidth()) / 2, (ROWS - bitmap.getHeight()) / 2);
    }
}