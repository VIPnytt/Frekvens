#include "config/constants.h"

#if MODE_METABALLS

#include "extensions/MicrophoneExtension.h"
#include "modes/MetaballsMode.h"
#include "services/DisplayService.h"

void MetaballsMode::setup()
{
    for (int16_t i = 0; i < sizeof(contributions); ++i)
    {
        contributions[i] = ((UINT8_MAX - i) * (UINT8_MAX - i) * (1 << 6)) >> 16;
    }
    for (Ball &ball : balls)
    {
        ball.x = random(COLUMNS);
        ball.y = random(ROWS);
        ball.xVelocity = random(1, multiplier) * speed * (random(2) * 2 - 1);
        ball.yVelocity = random(1, multiplier) * speed * (random(2) * 2 - 1);
    }
}

void MetaballsMode::wake()
{
    radius = min<float>(COLUMNS * Display.getCellRatio(), ROWS) / 5.0f;
    radiusSq = radius * radius;
}

void MetaballsMode::handle()
{
#if EXTENSION_MICROPHONE
    if (Microphone->play())
#endif
    {
        for (const Ball &ball : balls)
        {
            const uint8_t
                xMin = max<int8_t>(ball.x - radius - max<float>(ball.xVelocity, 0), 0),
                yMin = max<int8_t>(ball.y - radius - max<float>(ball.yVelocity, 0), 0),
                xMax = min<int8_t>(ceil(ball.x + radius - min<float>(ball.xVelocity, 0)), COLUMNS - 1),
                yMax = min<int8_t>(ceil(ball.y + radius - min<float>(ball.yVelocity, 0)), ROWS - 1);
            for (uint8_t x = xMin; x <= xMax; ++x)
            {
                for (uint8_t y = yMin; y <= yMax; ++y)
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
                            brightness = min<uint8_t>(brightness + contributions[(uint8_t)(distSq * UINT8_MAX / radiusSq)], UINT8_MAX);
                            if (brightness >= UINT8_MAX)
                            {
                                break;
                            }
                        }
                    }
                    Display.setPixel(x, y, brightness);
                }
            }
        }
        for (Ball &ball : balls)
        {
            ball.x += ball.xVelocity;
            ball.y += ball.yVelocity;
            if (ball.x < 0)
            {
                ball.x = 0;
                ball.xVelocity = random(1, multiplier) * speed;
            }
            else if (ball.x > COLUMNS - 1)
            {
                ball.x = COLUMNS - 1;
                ball.xVelocity = random(1, multiplier) * -speed;
            }
            if (ball.y < 0)
            {
                ball.y = 0;
                ball.yVelocity = random(1, multiplier) * speed;
            }
            else if (ball.y > ROWS - 1)
            {
                ball.y = ROWS - 1;
                ball.yVelocity = random(1, multiplier) * -speed;
            }
        }
    }
}

#endif // MODE_METABALLS
