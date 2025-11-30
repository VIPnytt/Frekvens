#include "config/constants.h"

#if MODE_FIREWORK

#include "extensions/MicrophoneExtension.h"
#include "modes/FireworkMode.h"
#include "services/DisplayService.h"

void FireworkMode::handle()
{
    switch (stage)
    {
    case 1:
        launching();
        break;
    case 2:
        exploding();
        break;
    case 3:
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
    if (Microphone->isTriggered())
#endif // EXTENSION_MICROPHONE
    {
        rocketX = random(GRID_COLUMNS);
        rocketY = GRID_ROWS;
        stage = 1;
    }
}

void FireworkMode::launching()
{
    if (millis() - lastMillis > (1 << 6))
    {
        if (rocketY < GRID_ROWS)
        {
            Display.setPixel(rocketX, rocketY, 0);
        }
        --rocketY;
        Display.setPixel(rocketX, rocketY, 1);
        lastMillis = millis();
        if (rocketY <= random(GRID_ROWS / 2))
        {
            radius = 0;
#if PITCH_VERTICAL == PITCH_HORIZONTAL
            maxRadius = random(1, min(GRID_COLUMNS, GRID_ROWS) / 2);
#else
            maxRadius = random(2, min(GRID_COLUMNS * PITCH_HORIZONTAL / (float)PITCH_VERTICAL, GRID_ROWS / (float)PITCH_HORIZONTAL * PITCH_VERTICAL) / 2);
#endif // PITCH_VERTICAL == PITCH_HORIZONTAL
            stage = 2;
        }
    }
}

void FireworkMode::exploding()
{
    if (millis() - lastMillis > INT8_MAX)
    {
        ++radius;
        Display.drawEllipse(rocketX, rocketY, radius, 1, true, UINT8_MAX / maxRadius * radius);
        lastMillis = millis();
        if (radius >= maxRadius)
        {
            brightness = UINT8_MAX;
            stage = 3;
        }
    }
}

void FireworkMode::fading()
{
    if (random(3) == 0)
    {
        --brightness;
    }
    Display.drawEllipse(rocketX, rocketY, radius, 1, true, brightness);
    lastMillis = millis();
    if (brightness <= 0)
    {
        stage = 0;
        Display.clearFrame();
    }
}

#endif // MODE_FIREWORK
