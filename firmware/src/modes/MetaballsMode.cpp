#include "extensions/MicrophoneExtension.h"
#include "modes/MetaballsMode.h"
#include "services/DisplayService.h"

void MetaballsMode::wake()
{
    const float r = min((float)(COLUMNS * Display.getCellRatio()), (float)ROWS) / 5.0f;
    radiusSq = r * r;
    for (Ball &ball : balls)
    {
        ball.x = random(COLUMNS);
        ball.y = random(ROWS);
        ball.xVelocity = random(1, 1 << 3) * speed * (random(2) * 2 - 1);
        ball.yVelocity = random(1, 1 << 3) * speed * (random(2) * 2 - 1);
    }
}

void MetaballsMode::handle()
{
#if EXTENSION_MICROPHONE
    if (Microphone->play())
#endif
    {
        for (uint8_t x = 0; x < COLUMNS; ++x)
        {
            for (uint8_t y = 0; y < ROWS; ++y)
            {
                uint8_t brightness = 0;
                for (const Ball &ball : balls)
                {
                    const float
                        xDist = (ball.x - x) * Display.getCellRatio(),
                        yDist = ball.y - y,
                        distSq = xDist * xDist + yDist * yDist;
                    if (distSq < radiusSq)
                    {
                        const float ratio = distSq / radiusSq;
                        brightness = min((uint16_t)(brightness + (1.0f - ratio) * (1.0f - ratio) * (1 << 6)), (uint16_t)UINT8_MAX);
                        if (brightness >= UINT8_MAX)
                        {
                            break;
                        }
                    }
                }
                Display.setPixel(x, y, brightness);
            }
        }
        for (Ball &ball : balls)
        {
            ball.x += ball.xVelocity;
            ball.y += ball.yVelocity;
            if (ball.x < 0)
            {
                ball.x = 0;
                ball.xVelocity = random(1, 1 << 3) * speed;
            }
            else if (ball.x > COLUMNS - 1)
            {
                ball.x = COLUMNS - 1;
                ball.xVelocity = random(1, 1 << 3) * -speed;
            }
            if (ball.y < 0)
            {
                ball.y = 0;
                ball.yVelocity = random(1, 1 << 3) * speed;
            }
            else if (ball.y > ROWS - 1)
            {
                ball.y = ROWS - 1;
                ball.yVelocity = random(1, 1 << 3) * -speed;
            }
        }
    }
}
