#if MODE_LEAFFALL

#include "modes/LeafFallMode.h"

#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS * GRID_ROWS >= 20U,
              __STRING(MODE_LEAFFALL) " is not compatible with this device's display size.");

/**
 * @brief Initializes leaf positions, brightness, movement delays, and display state.
 */
void LeafFallMode::begin()
{
    for (Leaf &leaf : leaves)
    {
        leaf.x = random(GRID_COLUMNS);
        leaf.y = random(GRID_ROWS);
        leaf.brightness = random(0b1U << 4U, 0b1U << 8U);
        leaf.delay = random(UINT8_MAX, 600);
        leaf.lastMillis = millis();
    }
    Display.fillFrame(0U);
}

/**
 * @brief Advances each leaf whose movement delay has elapsed.
 *
 * Clears the previous position, moves leaves downward or diagonally, respawns
 * leaves at an available position at the top, and updates their brightness
 * and movement timing.
 */
void LeafFallMode::handle()
{
    for (Leaf &leaf : leaves)
    {
        if (millis() - leaf.lastMillis > leaf.delay)
        {
            Display.setPixel(leaf.x, leaf.y, 0U);
            if (leaf.y + 1 >= GRID_ROWS)
            {
#if EXTENSION_MICROPHONE
                leaf.brightness = Extensions.Microphone().isTriggered() ? random(0b1U << 4U, 0b1U << 8U) : 0U;
#else
                leaf.brightness = random(0b1U << 4U, 0b1U << 8U);
#endif // EXTENSION_MICROPHONE
                leaf.y = 0U;
                do
                {
                    leaf.x = random(GRID_COLUMNS);
                } while (Display.getPixel(leaf.x, leaf.y) != 0U);
            }
            else if (leaf.x != 0U && random(4) == 0)
            {
                --leaf.x;
            }
            else if (leaf.x < GRID_COLUMNS - 1U && random(4) == 0)
            {
                ++leaf.x;
            }
            else
            {
                ++leaf.y;
            }
#if EXTENSION_MICROPHONE
            if (leaf.brightness != 0U)
            {
                Display.setPixel(leaf.x, leaf.y, leaf.brightness);
            }
#else
            Display.setPixel(leaf.x, leaf.y, leaf.brightness);
#endif // EXTENSION_MICROPHONE
            leaf.delay = random(UINT8_MAX, 600);
            leaf.lastMillis = millis();
        }
    }
}

#endif // MODE_LEAFFALL
