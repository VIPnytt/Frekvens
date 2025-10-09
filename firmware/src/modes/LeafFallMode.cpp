#include "config/constants.h"

#if MODE_LEAFFALL

#include "modes/LeafFallMode.h"
#include "services/DisplayService.h"

void LeafFallMode::wake()
{
    for (Leaf &leaf : leaves)
    {
        leaf.x = random(COLUMNS);
        leaf.y = random(ROWS);
        leaf.brightness = random(25, 1U << 8);
        leaf.delay = random(UINT8_MAX, 600);
        leaf.lastMillis = millis();
    }
    Display.clear();
}

void LeafFallMode::handle()
{
    for (Leaf &leaf : leaves)
    {
        if (millis() - leaf.lastMillis > leaf.delay)
        {
            Display.setPixel(leaf.x, leaf.y, 0);
            if (leaf.y + 1 >= ROWS)
            {
                leaf.brightness = random(25, 1U << 8);
                leaf.y = 0;
                do
                {
                    leaf.x = random(COLUMNS);
                } while (Display.getPixel(leaf.x, leaf.y));
            }
            else if (leaf.x > 0 && random(4) == 0)
            {
                --leaf.x;
            }
            else if (leaf.x < COLUMNS - 1 && random(4) == 0)
            {
                ++leaf.x;
            }
            else
            {
                ++leaf.y;
            }
            Display.setPixel(leaf.x, leaf.y, leaf.brightness);
            leaf.delay = random(UINT8_MAX, 600);
            leaf.lastMillis = millis();
        }
    }
}

#endif // MODE_LEAFFALL
