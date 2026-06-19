#if MODE_RAIN

#include "modes/RainMode.h"

#include "services/DisplayService.h"

static_assert(GRID_COLUMNS >= 3U, __STRING(MODE_RAIN) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 5U, __STRING(MODE_RAIN) " is not compatible with this device's display size.");

void RainMode::begin()
{
    for (Drop &drop : drops)
    {
        drop.x = random(GRID_COLUMNS);
        drop.y = random(GRID_ROWS);
        drop.length = random(GRID_ROWS / 5U, GRID_ROWS / 3U);
        drop.delay = random(INT8_MAX, UINT8_MAX);
        drop.lastMillis = millis();
    }
    Display.clearFrame();
}

void RainMode::handle()
{
    for (Drop &drop : drops)
    {
        if (millis() - drop.lastMillis > drop.delay)
        {
            if (drop.y - drop.length >= static_cast<int8_t>(GRID_ROWS))
            {
                const uint8_t newX{static_cast<uint8_t>(random(GRID_COLUMNS))};
                if (std::none_of(drops.begin(), drops.end(), [&](const Drop &_drop) { return _drop.x == newX; }))
                {
                    drop.x = newX;
                }
                drop.y = 0U;
                drop.length = random(GRID_ROWS / 5U, GRID_ROWS / 3U);
                drop.delay = random(100, UINT8_MAX);
            }
            else
            {
                ++drop.y;
            }
            for (uint8_t n{0U}; n <= drop.length; ++n)
            {
                if (drop.y + n - drop.length >= 0 && drop.y + n - drop.length < static_cast<int8_t>(GRID_ROWS))
                {
                    Display.setPixel(drop.x, drop.y + n - drop.length, UINT8_MAX / drop.length * n);
                }
            }
            drop.lastMillis = millis();
        }
    }
}

#endif // MODE_RAIN
