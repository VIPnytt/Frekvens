#if MODE_METABALLS

#include "modes/MetaballsMode.h"

#include "extensions/MicrophoneExtension.h"
#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)
#include "services/ExtensionsService.h"

void MetaballsMode::begin()
{
    for (uint16_t i = 0; i < contributions.size(); ++i)
    {
        contributions[i] = ((UINT8_MAX - i) * (UINT8_MAX - i) * (1U << 6U)) >> 16U;
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
    if (!Extensions.Microphone().isTriggered())
    {
        return;
    }
#endif // EXTENSION_MICROPHONE
#if PITCH_HORIZONTAL != PITCH_VERTICAL
    const bool rotated = (static_cast<uint8_t>(Display.getOrientation()) & 1U) != 0;
    const float xRatio = static_cast<float>(2 * (rotated ? PITCH_VERTICAL : PITCH_HORIZONTAL)) /
                         static_cast<float>(PITCH_VERTICAL + PITCH_HORIZONTAL);
    const float yRatio = static_cast<float>(2 * (rotated ? PITCH_HORIZONTAL : PITCH_VERTICAL)) /
                         static_cast<float>(PITCH_VERTICAL + PITCH_HORIZONTAL);
#endif // PITCH_HORIZONTAL == PITCH_VERTICAL
    for (const Ball &ball : balls)
    {
        const uint8_t xMin =
            static_cast<uint8_t>(max<int8_t>(static_cast<int8_t>(ball.x - radius - max(ball.xVelocity, 0.0F)), 0));
        const uint8_t yMin =
            static_cast<uint8_t>(max<int8_t>(static_cast<int8_t>(ball.y - radius - max(ball.yVelocity, 0.0F)), 0));
        const uint8_t xMax = static_cast<uint8_t>(
            min<int8_t>(static_cast<int8_t>(ceilf(ball.x + radius - min(ball.xVelocity, 0.0F))), GRID_COLUMNS - 1));
        const uint8_t yMax = static_cast<uint8_t>(
            min<int8_t>(static_cast<int8_t>(ceilf(ball.y + radius - min(ball.yVelocity, 0.0F))), GRID_ROWS - 1));
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
                            brightness + contributions[static_cast<uint8_t>(distanceSq * (1U << 6U) / radiusSq)],
                            UINT8_MAX);
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

#endif // MODE_METABALLS
