#include "config/constants.h"

#if MODE_ARROW

#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h"
#include "modes/ArrowMode.h"
#include "services/DisplayService.h"

void ArrowMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > 150 && Microphone->play())
#else
    if (millis() - lastMillis > 150)
#endif
    {
        lastMillis = millis();

        Display.clear();
        BitmapHandler bitmap = BitmapHandler(arrow[index]);
        bitmap.draw((COLUMNS - bitmap.getWidth()) / 2, ROWS - bitmap.getHeight());

        direction ? ++index : --index;
        if (index <= 0 || index >= arrow.size() - 1)
        {
            direction = !direction;
        }
    }
}

#endif // MODE_ARROW
