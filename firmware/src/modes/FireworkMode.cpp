#if MODE_FIREWORK

#include "modes/FireworkMode.h"

#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

static_assert(GRID_COLUMNS >= 4U, __STRING(MODE_FIREWORK) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 4U, __STRING(MODE_FIREWORK) " is not compatible with this device's display size.");

void FireworkMode::handle()
{
    switch (stage)
    {
    case 1U:
        launching();
        break;
    case 2U:
        exploding();
        break;
    case 3U:
        fading();
        break;
    default:
        pad();
        break;
    }
}

void FireworkMode::pad()
{
#if EXTENSION_MICROPHONE
    if (Extensions.Microphone().isTriggered())
#endif // EXTENSION_MICROPHONE
    {
        rocketX = random(GRID_COLUMNS);
        rocketY = GRID_ROWS;
        stage = 1U;
    }
}

/**
 * @brief Advances the rocket toward the upper half of the display and starts the explosion stage.
 */
void FireworkMode::launching()
{
    if (millis() - lastMillis > (1U << 6U))
    {
        if (rocketY < GRID_ROWS)
        {
            Display.setPixel(rocketX, rocketY, 0U);
        }
        --rocketY;
        Display.setPixel(rocketX, rocketY, 1U);
        lastMillis = millis();
        if (rocketY <= random(GRID_ROWS / 2))
        {
            radius = 0U;
#if PITCH_VERTICAL == PITCH_HORIZONTAL
            maxRadius = random(1, min(GRID_COLUMNS, GRID_ROWS) / 2);
#else
            maxRadius = random(2,
                               min(GRID_COLUMNS * PITCH_HORIZONTAL / static_cast<float>(PITCH_VERTICAL),
                                   GRID_ROWS / static_cast<float>(PITCH_HORIZONTAL * PITCH_VERTICAL)) /
                                   2.0F);
#endif // PITCH_VERTICAL == PITCH_HORIZONTAL
            stage = 2U;
        }
    }
}

/**
 * @brief Advances the firework explosion and renders its expanding ellipse.
 *
 * Transitions the firework to the fading stage when the maximum explosion radius
 * is reached.
 */
void FireworkMode::exploding()
{
    if (millis() - lastMillis > INT8_MAX)
    {
        ++radius;
        Display.drawEllipseSolid(rocketX, rocketY, radius, UINT8_MAX / maxRadius * radius);
        lastMillis = millis();
        if (radius >= maxRadius)
        {
            brightness = UINT8_MAX;
            stage = 3U;
        }
    }
}

/**
 * @brief Fades the firework explosion until it disappears.
 */
void FireworkMode::fading()
{
    if (random(3) == 0)
    {
        --brightness;
    }
    Display.drawEllipseSolid(rocketX, rocketY, radius, brightness);
    lastMillis = millis();
    if (brightness == 0U)
    {
        stage = 0U;
    }
}

#endif // MODE_FIREWORK
