#if MODE_GLITTER

#include "modes/GlitterMode.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

void GlitterMode::handle()
{
#if EXTENSION_MICROPHONE
    if (Extensions.Microphone().isTriggered())
#endif // EXTENSION_MICROPHONE
    {
        Display.setPixel(random(GRID_COLUMNS), random(GRID_ROWS), random(1, 0b1U << 8U));
    }
}

#endif // MODE_GLITTER
