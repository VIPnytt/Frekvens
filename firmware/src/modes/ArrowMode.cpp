#if MODE_ARROW

#include "modes/ArrowMode.h"

#include "config/constants.h"        // NOLINT(misc-include-cleaner)
#include "handlers/BitmapHandler.h"  // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)
#include "services/ExtensionsService.h"

void ArrowMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        lastMillis = millis();
        Display.fillFrame(0U);
        if (index < arrows0.size())
        {
            const BitmapHandler bitmap(arrows0.at(index));
            bitmap.draw((GRID_COLUMNS - bitmap.getWidth()) / 2, GRID_ROWS - bitmap.getHeight());
        }
        else
        {
            const BitmapHandler bitmap(arrows1.at(index - arrows0.size()));
            bitmap.draw((GRID_COLUMNS - bitmap.getWidth()) / 2, GRID_ROWS - bitmap.getHeight());
        }
        direction ? ++index : --index;
        if (index == 0U || index >= arrows0.size() + arrows1.size() - 1U)
        {
            direction = !direction;
        }
    }
}

#endif // MODE_ARROW
