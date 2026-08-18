#if MODE_EQUALIZER

#include "modes/EqualizerMode.h"

#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

static_assert(GRID_COLUMNS >= 4U, __STRING(MODE_EQUALIZER) " is not compatible with this device's display size.");

/**
 * @brief Clears the separator columns between equalizer bars.
 */
void EqualizerMode::begin()
{
    for (uint8_t x{width}; x < GRID_COLUMNS; x += width + 1U)
    {
        Display.fillColumn(x, 0U);
    }
}

/**
 * @brief Updates the equalizer bars and redraws their current heights.
 *
 * Bar targets are randomized, with optional microphone input influencing target
 * selection when microphone support is enabled.
 */
void EqualizerMode::handle()
{
    if (millis() - lastMillis > (0b1U << 4U))
    {
        lastMillis = millis();
#if EXTENSION_MICROPHONE
        const bool sound{Extensions.Microphone().isTriggered()};
#endif // EXTENSION_MICROPHONE
        uint8_t idx{0U};
        for (std::pair<uint8_t, uint8_t> &bar : bars)
        {
            if (bar.second == bar.first)
            {
#if EXTENSION_MICROPHONE
                bar.second = sound ? random(GRID_ROWS) : GRID_ROWS - 1U;
#else
                bar.second = static_cast<uint8_t>(random(GRID_ROWS));
#endif // EXTENSION_MICROPHONE
            }
            else if (random(0b1U << 3U) == 0)
            {
                const uint8_t minX{static_cast<uint8_t>(idx * (width + 1U))};
                if (bar.first < bar.second)
                {
                    Display.drawLineHorizontal(minX, width, bar.first, 0U);
                    ++bar.first;
                }
                else if (bar.first > bar.second)
                {
                    --bar.first;
                }
                Display.drawRectangleSolid(
                    minX, width, bar.first, GRID_ROWS - 1U, UINT8_MAX * (GRID_ROWS - bar.first) / GRID_ROWS);
            }
            ++idx;
        }
    }
}

#endif // MODE_EQUALIZER
