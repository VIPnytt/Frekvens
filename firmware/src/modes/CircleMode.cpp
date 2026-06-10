#if MODE_CIRCLE

#include "modes/CircleMode.h"

#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)
#include "services/ExtensionsService.h"

static_assert(GRID_COLUMNS >= 3U, __STRING(MODE_CIRCLE) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 3U, __STRING(MODE_CIRCLE) " is not compatible with this device's display size.");

void CircleMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > (1U << 6U) && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis > (1U << 6U))
#endif // EXTENSION_MICROPHONE
    {
        bool _lit{lit};
        for (uint16_t _radius{radius}; _radius < uint16_t{maxRadius} + 1U; _radius += 3U)
        {
            Display.drawEllipse(x, y, static_cast<float>(_radius), false, _lit ? UINT8_MAX : 0U);
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
