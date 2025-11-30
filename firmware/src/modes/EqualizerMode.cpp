#include "config/constants.h"

#if MODE_EQUALIZER

#include "extensions/MicrophoneExtension.h"
#include "modes/EqualizerMode.h"
#include "services/DisplayService.h"

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
    if (millis() - lastMillis > (1 << 4))
    {
        lastMillis = millis();
#if EXTENSION_MICROPHONE
        const bool play = Microphone->isTriggered();
#endif // EXTENSION_MICROPHONE
        uint8_t minX = 0;
        for (Bar &bar : bars)
        {
            if (bar.target == bar.level)
            {
#if EXTENSION_MICROPHONE
                bar.target = play ? random(GRID_ROWS) : GRID_ROWS - 1;
#else
                bar.target = random(GRID_ROWS);
#endif // EXTENSION_MICROPHONE
            }
            else if (random(1 << 3) == 0)
            {
                const uint8_t maxX = minX + width - 1;
                if (bar.level < bar.target)
                {
                    for (uint8_t x = minX; x <= maxX; ++x)
                    {
                        Display.setPixel(x, bar.level, 0);
                    }
                    ++bar.level;
                }
                else if (bar.level > bar.target)
                {
                    --bar.level;
                }
                Display.drawRectangle(minX, bar.level, maxX, GRID_ROWS - 1, true, UINT8_MAX * (GRID_ROWS - bar.level) / GRID_ROWS);
            }
            minX += width + 1;
        }
    }
}

#endif // MODE_EQUALIZER
