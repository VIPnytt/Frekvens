#include "modes/RainMode.h"
#include "services/DisplayService.h"

void RainMode::wake()
{
    for (Drop &drop : drops)
    {
        drop.x = random(COLUMNS);
        drop.y = random(ROWS);
        drop.length = random(ROWS / 5, ROWS / 3);
        drop.delay = random(100, UINT8_MAX);
        drop.lastMillis = millis();
    }
    Display.clear();
}

void RainMode::handle()
{
    for (Drop &drop : drops)
    {
        if (millis() - drop.lastMillis > drop.delay)
        {
            if (drop.y - drop.length >= ROWS)
            {
                uint8_t newX;
                bool isUsedX = false;
                do
                {
                    isUsedX = false;
                    newX = random(COLUMNS);
                    for (const Drop &_drop : drops)
                    {
                        if (_drop.x == newX)
                        {
                            isUsedX = true;
                            break;
                        }
                    }
                } while (isUsedX);
                drop.x = newX;
                drop.y = 0;
                drop.length = random(ROWS / 5, ROWS / 3);
                drop.delay = random(100, UINT8_MAX);
            }
            else
            {
                ++drop.y;
            }
            for (uint8_t n = 0; n <= drop.length; ++n)
            {
                if (drop.y - drop.length + n >= 0 && drop.y - drop.length + n < ROWS)
                {
                    Display.setPixel(drop.x, drop.y - drop.length + n, UINT8_MAX / drop.length * n);
                }
            }
            drop.lastMillis = millis();
        }
    }
}
