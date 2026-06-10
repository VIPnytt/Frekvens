#if MODE_FIREWORK

#include "modes/FireworkMode.h"

#include "extensions/MicrophoneExtension.h"
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

void FireworkMode::exploding()
{
    if (millis() - lastMillis > INT8_MAX)
    {
        ++radius;
        Display.drawEllipse(rocketX, rocketY, radius, true, UINT8_MAX / maxRadius * radius);
        lastMillis = millis();
        if (radius >= maxRadius)
        {
            brightness = UINT8_MAX;
            stage = 3U;
        }
    }
}

void FireworkMode::fading()
{
    if (random(3) == 0)
    {
        --brightness;
    }
    Display.drawEllipse(rocketX, rocketY, radius, true, brightness);
    lastMillis = millis();
    if (brightness == 0U)
    {
        stage = 0U;
        Display.clearFrame();
    }
}

#endif // MODE_FIREWORK
