#if MODE_NOISE

#include "modes/NoiseMode.h"

#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)

void NoiseMode::handle()
{
    for (Dot &dot : dots)
    {
        if (millis() - dot.lastMillis > dot.delay)
        {
            Display.setPixel(dot.x, dot.y, 0);
            dot.x = random(GRID_COLUMNS);
            dot.y = random(GRID_ROWS);
            dot.delay = random(1U << 6U, INT8_MAX);
            Display.setPixel(dot.x, dot.y);
            dot.lastMillis = millis();
        }
    }
}

#endif // MODE_NOISE
