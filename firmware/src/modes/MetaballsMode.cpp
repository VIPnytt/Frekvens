#if MODE_METABALLS

#include "modes/MetaballsMode.h"

#include "extensions/MicrophoneExtension.h"
#include "services/DisplayService.h"

void MetaballsMode::configure()
{
    for (uint16_t i = 0; i < sizeof(contributions); ++i)
    {
        contributions[i] = ((UINT8_MAX - i) * (UINT8_MAX - i) * (1 << 6)) >> 16;
    }
    for (Ball &ball : balls)
    {
        ball.x = random(GRID_COLUMNS);
        ball.y = random(GRID_ROWS);
        ball.xVelocity = random(1, multiplier) * speed * (random(2) * 2 - 1);
        ball.yVelocity = random(1, multiplier) * speed * (random(2) * 2 - 1);
    }
}

void MetaballsMode::handle()
{
#if EXTENSION_MICROPHONE
    if (Microphone->isTriggered())
#endif // EXTENSION_MICROPHONE
    {
        const bool rotated = Display.getOrientation() % 2;
        const float
            xRatio = 2 * (rotated ? PITCH_VERTICAL : PITCH_HORIZONTAL) / (float)(PITCH_VERTICAL + PITCH_HORIZONTAL),
            yRatio = 2 * (rotated ? PITCH_HORIZONTAL : PITCH_VERTICAL) / (float)(PITCH_VERTICAL + PITCH_HORIZONTAL);
        for (const Ball &ball : balls)
        {
            const uint8_t
                xMin = max<int8_t>(ball.x - radius - max<float>(ball.xVelocity, 0), 0),
                yMin = max<int8_t>(ball.y - radius - max<float>(ball.yVelocity, 0), 0),
                xMax = min<int8_t>(ceil(ball.x + radius - min<float>(ball.xVelocity, 0)), GRID_COLUMNS - 1),
                yMax = min<int8_t>(ceil(ball.y + radius - min<float>(ball.yVelocity, 0)), GRID_ROWS - 1);
            for (uint8_t x = xMin; x <= xMax; ++x)
            {
                for (uint8_t y = yMin; y <= yMax; ++y)
                {
                    uint8_t brightness = 0;
                    for (const Ball &ball : balls)
                    {
                        const float
                            xDistance = (ball.x - x) * xRatio,
                            yDistance = (ball.y - y) * yRatio,
                            distanceSq = xDistance * xDistance + yDistance * yDistance;
                        if (distanceSq < radiusSq)
                        {
                            brightness = min<uint8_t>(brightness + contributions[(uint8_t)(distanceSq * UINT8_MAX / radiusSq)], UINT8_MAX);
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
            else if (ball.x > GRID_COLUMNS - 1)
            {
                ball.x = GRID_COLUMNS - 1;
                ball.xVelocity = random(1, multiplier) * -speed;
            }
            if (ball.y < 0)
            {
                ball.y = 0;
                ball.yVelocity = random(1, multiplier) * speed;
            }
            else if (ball.y > GRID_ROWS - 1)
            {
                ball.y = GRID_ROWS - 1;
                ball.yVelocity = random(1, multiplier) * -speed;
            }
        }
    }
}

#endif // MODE_METABALLS
