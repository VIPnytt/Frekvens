#include "config/constants.h"
#include "modes/NoiseMode.h"
#include "services/DisplayService.h"

void NoiseMode::handle()
{
    for (Dot &dot : dots)
    {
        if (millis() - dot.lastMillis > dot.delay)
        {
            Display.setPixel(dot.x, dot.y, 0);
            dot.x = random(COLUMNS);
            dot.y = random(ROWS);
            dot.delay = random(100, 200);
            Display.setPixel(dot.x, dot.y);
            dot.lastMillis = millis();
        }
    }
}
