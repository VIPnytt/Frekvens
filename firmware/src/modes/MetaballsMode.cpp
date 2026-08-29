#if MODE_METABALLS

#include "modes/MetaballsMode.h"

#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)
#include "services/ExtensionsService.h"

static_assert(GRID_COLUMNS * GRID_ROWS >= 50U,
              __STRING(MODE_METABALLS) " is not compatible with this device's display size.");

void MetaballsMode::begin()
{
    // Builds a lookup table of a single ball's brightness contribution by distance: full brightness
    // at the center (idx 0), fading quadratically to none at the edge (idx == falloffResolution).
    // Multiple overlapping balls add their contributions together, so peakBrightness is kept well
    // below UINT8_MAX to require several overlapping balls before a pixel reaches full brightness.
    constexpr float peakBrightness{64.0F};
    constexpr float span{static_cast<float>(falloffResolution) + 1.0F};
    for (size_t idx{0U}; idx < contributions.size(); ++idx)
    {
        const float normalizedDistance{static_cast<float>(falloffResolution - idx) / span};
        contributions[idx] = static_cast<uint8_t>(peakBrightness * normalizedDistance * normalizedDistance);
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
    const bool rotated{(static_cast<uint8_t>(Display.getOrientation()) & 0b1U) != 0U};
    const float xRatio{static_cast<float>(2U * (rotated ? PITCH_VERTICAL : PITCH_HORIZONTAL)) /
                       static_cast<float>(PITCH_VERTICAL + PITCH_HORIZONTAL)};
    const float yRatio{static_cast<float>(2U * (rotated ? PITCH_HORIZONTAL : PITCH_VERTICAL)) /
                       static_cast<float>(PITCH_VERTICAL + PITCH_HORIZONTAL)};
#endif // PITCH_HORIZONTAL != PITCH_VERTICAL
    for (const Ball &ball : balls)
    {
        const uint8_t yMax{static_cast<uint8_t>(
            min<int8_t>(static_cast<int8_t>(ceilf(ball.y + radius - min(ball.yVelocity, .0F))), GRID_ROWS - 1U))};
        const uint8_t yMin{
            static_cast<uint8_t>(max<int8_t>(static_cast<int8_t>(ball.y - radius - max(ball.yVelocity, .0F)), 0))};
        for (uint8_t x{
                 static_cast<uint8_t>(max<int8_t>(static_cast<int8_t>(ball.x - radius - max(ball.xVelocity, .0F)), 0))};
             x <= static_cast<uint8_t>(min<int8_t>(
                      static_cast<int8_t>(ceilf(ball.x + radius - min(ball.xVelocity, .0F))), GRID_COLUMNS - 1U));
             ++x)
        {
            for (uint8_t y{yMin}; y <= yMax; ++y)
            {
                uint8_t brightness{0U};
                for (const Ball &ball : balls)
                {
#if PITCH_HORIZONTAL == PITCH_VERTICAL
                    const float xDistance{ball.x - static_cast<float>(x)};
                    const float yDistance{ball.y - static_cast<float>(y)};
#else
                    const float xDistance{(ball.x - static_cast<float>(x)) * xRatio};
                    const float yDistance{(ball.y - static_cast<float>(y)) * yRatio};
#endif // PITCH_HORIZONTAL == PITCH_VERTICAL
                    const float distanceSq{(xDistance * xDistance) + (yDistance * yDistance)};
                    if (distanceSq < radiusSq)
                    {
                        brightness = static_cast<uint8_t>(min<uint16_t>(
                            static_cast<uint16_t>(brightness) +
                                contributions[static_cast<uint8_t>(min<float>(distanceSq * falloffResolution / radiusSq,
                                                                              static_cast<float>(falloffResolution)))],
                            UINT8_MAX));
                        if (brightness == UINT8_MAX)
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
        if (ball.x < .0F)
        {
            ball.x = .0F;
            ball.xVelocity = speed * static_cast<float>(random(1, multiplier));
        }
        else if (ball.x > GRID_COLUMNS - 1U)
        {
            ball.x = GRID_COLUMNS - 1U;
            ball.xVelocity = -speed * static_cast<float>(random(1, multiplier));
        }
        if (ball.y < .0F)
        {
            ball.y = .0F;
            ball.yVelocity = speed * static_cast<float>(random(1, multiplier));
        }
        else if (ball.y > GRID_ROWS - 1U)
        {
            ball.y = GRID_ROWS - 1U;
            ball.yVelocity = -speed * static_cast<float>(random(1, multiplier));
        }
    }
}

#endif // MODE_METABALLS
