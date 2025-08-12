#include "config/constants.h"
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
    if (Microphone->play())
    {
#endif
        rocketX = random(COLUMNS - 1);
        rocketY = ROWS;
        stage = 1;
#if EXTENSION_MICROPHONE
    }
#endif
}

void FireworkMode::launching()
{
    if (millis() - lastMillis > 90)
    {
        if (rocketY < ROWS)
        {
            Display.setPixel(rocketX, rocketY, 0);
        }
        --rocketY;
        Display.setPixel(rocketX, rocketY, rocketY + 1);
        lastMillis = millis();
        if (rocketY <= random(ROWS / 2))
        {
            radius = 0;
#if defined(CELL_HEIGHT) && defined(CELL_WIDTH)
            maxRadius = random(min(COLUMNS * CELL_WIDTH / (float)CELL_HEIGHT, ROWS / (float)CELL_WIDTH * CELL_HEIGHT) / 2);
#else
            maxRadius = random(min(COLUMNS, ROWS) / 2);
#endif
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
        Display.clear();
    }
}
