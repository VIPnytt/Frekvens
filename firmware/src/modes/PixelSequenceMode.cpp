#if MODE_PIXELSEQUENCE

#include "modes/PixelSequenceMode.h"

#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

/**
 * @brief Initializes the pixel sequence mapping.
 *
 * Associates each display pixel identifier with its sequential index.
 */
void PixelSequenceMode::configure()
{
    for (uint16_t pixel{0U}; pixel < DisplayService::pixels.size(); ++pixel)
    {
        pixels[DisplayService::pixels[pixel]] = pixel;
    }
}

/**
 * @brief Advances the pixel sequence after the update interval has elapsed.
 *
 * When microphone support is enabled, an update also requires a microphone trigger.
 * The current pixel is set according to the active sequence phase, and the sequence
 * wraps around while toggling that phase after all pixels have been processed.
 */
void PixelSequenceMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > INT8_MAX && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis > INT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        Display.setPixel(pixels[idx], lit ? UINT8_MAX : 0U);
        if (++idx == pixels.size())
        {
            idx = 0U;
            lit = !lit;
        }
        lastMillis = millis();
    }
}

#endif // MODE_PIXELSEQUENCE
