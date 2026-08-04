#if MODE_PIXELSEQUENCE

#include "modes/PixelSequenceMode.h"

#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

void PixelSequenceMode::configure()
{
    for (uint16_t pixel{0U}; pixel < DisplayService::pixels.size(); ++pixel)
    {
        pixels[DisplayService::pixels[pixel]] = pixel;
    }
}

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
