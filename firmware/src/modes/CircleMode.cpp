#if MODE_CIRCLE

#include "modes/CircleMode.h"

#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)
#include "services/ExtensionsService.h"

void CircleMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > (1UL << 6U) && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis > (1UL << 6U))
#endif // EXTENSION_MICROPHONE
    {
        bool _lit = lit;
        for (uint8_t i = 0; i < static_cast<uint8_t>(((maxRadius - radius) / 3U) + 1U); ++i)
        {
            Display.drawEllipse(x, y, static_cast<float>(radius + (i * 3U)), 1.25F, false, _lit ? UINT8_MAX : 0);
            _lit = !_lit;
        }
        lastMillis = millis();
        ++radius;
        if (radius > 3)
        {
            radius = 1;
            lit = !lit;
        }
    }
}

#endif // MODE_CIRCLE
