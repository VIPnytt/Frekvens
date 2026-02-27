#if MODE_CIRCLE

#include "modes/CircleMode.h"

#include "extensions/MicrophoneExtension.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"        // NOLINT(misc-include-cleaner)

void CircleMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > (1UL << 6U) && Microphone->isTriggered())
#else
    if (millis() - lastMillis > (1UL << 6U))
#endif // EXTENSION_MICROPHONE
    {
        bool _lit = lit;
        for (uint8_t _radius = radius; _radius <= maxRadius; _radius += 3)
        {
            Display.drawEllipse(x, y, _radius, 1.25F, false, _lit ? UINT8_MAX : 0);
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
