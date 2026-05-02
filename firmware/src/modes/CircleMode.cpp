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
        for (uint16_t _radius = radius; _radius <= maxRadius; _radius += 3U)
        {
            Display.drawEllipse(x, y, static_cast<float>(_radius), 1.0F, false, _lit ? UINT8_MAX : 0);
            _lit = !_lit;
        }
        lastMillis = millis();
        ++radius;
        if (radius > 3U)
        {
            radius = 1U;
            lit = !lit;
        }
    }
}

#endif // MODE_CIRCLE
