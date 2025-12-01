#if MODE_ARROW

#include "config/constants.h"
#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h"
#include "modes/ArrowMode.h"
#include "services/DisplayService.h"

void ArrowMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Microphone->isTriggered())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();

        Display.clearFrame();
        BitmapHandler bitmap = BitmapHandler(arrow[index]);
        bitmap.draw((GRID_COLUMNS - bitmap.getWidth()) / 2, GRID_ROWS - bitmap.getHeight());

        direction ? ++index : --index;
        if (index <= 0 || index >= arrow.size() - 1)
        {
            direction = !direction;
        }
    }
}

#endif // MODE_ARROW
