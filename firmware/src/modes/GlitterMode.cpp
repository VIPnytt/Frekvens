#if MODE_GLITTER

#include "modes/GlitterMode.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

/**
 * @brief Sets a randomly selected display pixel to a random brightness when activated.
 */
void GlitterMode::handle()
{
#if EXTENSION_MICROPHONE
    if (Extensions.Microphone().isTriggered())
#endif // EXTENSION_MICROPHONE
    {
        Display.setPixel(random(GRID_COLUMNS * GRID_ROWS), static_cast<uint8_t>(random(1, 0b1U << 8U)));
    }
}

#endif // MODE_GLITTER
