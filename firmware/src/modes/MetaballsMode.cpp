#if MODE_METABALLS

#include "modes/MetaballsMode.h"

#include "extensions/MicrophoneExtension.h"
#include "services/DisplayService.h"

void MetaballsMode::configure()
{
    for (uint16_t i = 0; i < std::size(contributions); ++i)
    {
        contributions[i] = ((UINT8_MAX - i) * (UINT8_MAX - i) * (1 << 6)) >> 16;
    }
    for (Ball &ball : balls)
    {
        ball.x = static_cast<float>(random(GRID_COLUMNS));
        ball.y = static_cast<float>(random(GRID_ROWS));
        ball.xVelocity = speed * static_cast<float>(random(1, multiplier) * ((random(2) * 2) - 1));
        ball.yVelocity = speed * static_cast<float>(random(1, multiplier) * ((random(2) * 2) - 1));
    }
}

void MetaballsMode::handle()
{
#if EXTENSION_MICROPHONE
    if (Microphone->isTriggered())
#endif // EXTENSION_MICROPHONE
    {
#if PITCH_HORIZONTAL != PITCH_VERTICAL
        const bool rotated = (Display.getOrientation() % 2) != 0;
        const float xRatio = static_cast<float>(2 * (rotated ? PITCH_VERTICAL : PITCH_HORIZONTAL)) /
                             static_cast<float>(PITCH_VERTICAL + PITCH_HORIZONTAL);
        const float yRatio = static_cast<float>(2 * (rotated ? PITCH_HORIZONTAL : PITCH_VERTICAL)) /
                             static_cast<float>(PITCH_VERTICAL + PITCH_HORIZONTAL);
#endif // PITCH_HORIZONTAL == PITCH_VERTICAL
        for (const Ball &ball : balls)
        {
            const uint8_t xMin = max<int8_t>(ball.x - radius - max<float>(ball.xVelocity, 0), 0);
            const uint8_t yMin = max<int8_t>(ball.y - radius - max<float>(ball.yVelocity, 0), 0);
            const uint8_t xMax = min<int8_t>(ceilf(ball.x + radius - min<float>(ball.xVelocity, 0)), GRID_COLUMNS - 1);
            const uint8_t yMax = min<int8_t>(ceilf(ball.y + radius - min<float>(ball.yVelocity, 0)), GRID_ROWS - 1);
            for (uint8_t x = xMin; x <= xMax; ++x)
            {
                for (uint8_t y = yMin; y <= yMax; ++y)
                {
                    uint8_t brightness = 0;
                    for (const Ball &ball : balls)
                    {
#if PITCH_HORIZONTAL == PITCH_VERTICAL
                        const float xDistance = (ball.x - static_cast<float>(x));
                        const float yDistance = (ball.y - static_cast<float>(y));
#else
                        const float xDistance = (ball.x - static_cast<float>(x)) * xRatio;
                        const float yDistance = (ball.y - static_cast<float>(y)) * yRatio;
#endif // PITCH_HORIZONTAL == PITCH_VERTICAL
                        const float distanceSq = (xDistance * xDistance) + (yDistance * yDistance);
                        if (distanceSq < radiusSq)
                        {
                            brightness = min<uint8_t>(
                                brightness + contributions[(uint8_t)(distanceSq * UINT8_MAX / radiusSq)], UINT8_MAX);
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
                ball.xVelocity = speed * static_cast<float>(random(1, multiplier));
            }
            else if (ball.x > GRID_COLUMNS - 1)
            {
                ball.x = GRID_COLUMNS - 1;
                ball.xVelocity = -speed * static_cast<float>(random(1, multiplier));
            }
            if (ball.y < 0)
            {
                ball.y = 0;
                ball.yVelocity = speed * static_cast<float>(random(1, multiplier));
            }
            else if (ball.y > GRID_ROWS - 1)
            {
                ball.y = GRID_ROWS - 1;
                ball.yVelocity = -speed * static_cast<float>(random(1, multiplier));
            }
        }
    }
}

#endif // MODE_METABALLS
