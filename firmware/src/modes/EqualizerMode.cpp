#if MODE_EQUALIZER

#include "modes/EqualizerMode.h"

#include "extensions/MicrophoneExtension.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

static_assert(GRID_COLUMNS >= 4U, __STRING(MODE_EQUALIZER) " is not compatible with this device's display size.");

void EqualizerMode::begin()
{
    for (uint8_t x{width}; x < GRID_COLUMNS; x += width + 1U)
    {
        for (uint8_t y{0U}; y < GRID_ROWS; ++y)
        {
            Display.setPixel(x, y, 0U);
        }
    }
}

void EqualizerMode::handle()
{
    if (millis() - lastMillis > (1U << 4U))
    {
        lastMillis = millis();
#if EXTENSION_MICROPHONE
        const bool play{Extensions.Microphone().isTriggered()};
#endif // EXTENSION_MICROPHONE
        for (size_t i{0U}; i < bars.size(); ++i)
        {
            Bar &bar = bars[i];
            if (bar.target == bar.level)
            {
#if EXTENSION_MICROPHONE
                bar.target = play ? random(GRID_ROWS) : GRID_ROWS - 1;
#else
                bar.target = random(GRID_ROWS);
#endif // EXTENSION_MICROPHONE
            }
            else if (random(0b1U << 3U) == 0)
            {
                const uint8_t minX{static_cast<uint8_t>(i * (width + 1U))};
                const uint8_t maxX{static_cast<uint8_t>(minX + width)};
                if (bar.level < bar.target)
                {
                    for (uint8_t x{minX}; x < maxX; ++x)
                    {
                        Display.setPixel(x, bar.level, 0U);
                    }
                    ++bar.level;
                }
                else if (bar.level > bar.target)
                {
                    --bar.level;
                };
                for (uint8_t x{minX}; x < maxX; ++x)
                {
                    for (uint8_t y{bar.level}; y < GRID_ROWS; ++y)
                    {
                        Display.setPixel(x, y, UINT8_MAX * (GRID_ROWS - bar.level) / GRID_ROWS);
                    }
                }
            }
        }
    }
}

#endif // MODE_EQUALIZER
