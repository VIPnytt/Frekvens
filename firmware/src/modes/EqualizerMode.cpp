#if MODE_EQUALIZER

#include "modes/EqualizerMode.h"

#include "extensions/MicrophoneExtension.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

void EqualizerMode::begin()
{
    for (uint8_t x = width; x < GRID_COLUMNS; x += width + 1)
    {
        for (uint8_t y = 0; y < GRID_ROWS; ++y)
        {
            Display.setPixel(x, y, 0);
        }
    }
}

void EqualizerMode::handle()
{
    if (millis() - lastMillis > (1UL << 4U))
    {
        lastMillis = millis();
#if EXTENSION_MICROPHONE
        const bool play = Extensions.Microphone().isTriggered();
#endif // EXTENSION_MICROPHONE
        for (size_t i = 0; i < bars.size(); ++i)
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
            else if (random(1U << 3U) == 0)
            {
                const uint8_t minX = static_cast<uint8_t>(i * (width + 1));
                const uint8_t maxX = minX + width - 1;
                if (bar.level < bar.target)
                {
                    for (size_t x = minX; x <= maxX; ++x)
                    {
                        Display.setPixel(static_cast<uint8_t>(x), bar.level, 0);
                    }
                    ++bar.level;
                }
                else if (bar.level > bar.target)
                {
                    --bar.level;
                }
                Display.drawRectangle(
                    minX, bar.level, maxX, GRID_ROWS - 1, true, UINT8_MAX * (GRID_ROWS - bar.level) / GRID_ROWS);
            }
        }
    }
}

#endif // MODE_EQUALIZER
