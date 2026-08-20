#if MODE_RAIN

#include "modes/RainMode.h"

#include "services/DisplayService.h"

#include <algorithm>

static_assert(GRID_COLUMNS >= 3U, __STRING(MODE_RAIN) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 5U, __STRING(MODE_RAIN) " is not compatible with this device's display size.");

/**
 * @brief Initializes raindrop positions, lengths, delays, and timestamps, then clears the display frame.
 */
void RainMode::begin()
{
    for (Drop &drop : drops)
    {
        drop.x = static_cast<uint8_t>(random(GRID_COLUMNS));
        drop.y = static_cast<uint8_t>(random(GRID_ROWS));
        drop.length = static_cast<uint8_t>(random(GRID_ROWS / 5U, GRID_ROWS / 3U));
        drop.delay = static_cast<uint8_t>(random(INT8_MAX, UINT8_MAX));
        drop.lastMillis = millis();
    }
    Display.fillFrame(0U);
}

/**
 * @brief Advances and renders raindrops whose delays have elapsed.
 *
 * Raindrops that leave the display are repositioned with randomized length and delay
 * before their visible segments are rendered.
 */
void RainMode::handle()
{
    for (Drop &drop : drops)
    {
        if (millis() - drop.lastMillis > drop.delay)
        {
            if (static_cast<int>(drop.y) - static_cast<int>(drop.length) >= static_cast<int>(GRID_ROWS))
            {
                const uint8_t newX{static_cast<uint8_t>(random(GRID_COLUMNS))};
                if (std::none_of(drops.begin(), drops.end(), [&](const Drop &_drop) { return _drop.x == newX; }))
                {
                    drop.x = newX;
                }
                drop.y = 0U;
                drop.length = static_cast<uint8_t>(random(GRID_ROWS / 5U, GRID_ROWS / 3U));
                drop.delay = static_cast<uint8_t>(random(INT8_MAX, UINT8_MAX));
            }
            else
            {
                ++drop.y;
            }
            for (uint8_t n{0U}; n <= drop.length; ++n)
            {
                if (static_cast<int>(drop.y + n) - static_cast<int>(drop.length) >= 0 &&
                    static_cast<int>(drop.y + n) - static_cast<int>(drop.length) < static_cast<int>(GRID_ROWS))
                {
                    Display.setPixel(drop.x, drop.y + n - drop.length, UINT8_MAX * n / drop.length);
                }
            }
            drop.lastMillis = millis();
        }
    }
}

#endif // MODE_RAIN
