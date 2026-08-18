#if MODE_BINARYCLOCK

#include "modes/BinaryClockMode.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"

static_assert(GRID_COLUMNS >= 12U, __STRING(MODE_BINARYCLOCK) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 14U, __STRING(MODE_BINARYCLOCK) " is not compatible with this device's display size.");

void BinaryClockMode::begin() { pending = true; }

/**
 * @brief Updates the binary clock display with the current local time.
 */
void BinaryClockMode::handle()
{
    if (getLocalTime(&local))
    {
        if (pending)
        {
            Display.fillFrame(0U);
        }
        if (second != local.tm_sec || pending)
        {
            second = static_cast<uint8_t>(local.tm_sec);
            draw((GRID_ROWS / 2U) + 3U, second);
        }
        if (minute != local.tm_min || pending)
        {
            minute = local.tm_min;
            draw((GRID_ROWS / 2U) - 2U, minute);
        }
        if (hour != local.tm_hour || pending)
        {
            hour = local.tm_hour;
            draw((GRID_ROWS / 2U) - 7U, hour);
            pending = false;
        }
    }
}

/**
 * @brief Draws a six-bit binary value on the display.
 *
 * @param y Top row of the binary bars.
 * @param digit Value whose six least significant bits are rendered.
 */
void BinaryClockMode::draw(uint8_t y, uint8_t digit)
{
    for (uint8_t idx{0U}; idx < 6U; ++idx)
    {
        const uint8_t x{static_cast<uint8_t>((GRID_COLUMNS / 2U) + 4U - (idx * 2U))};
        const uint8_t brightness = (digit & (0b1U << idx)) == 0U ? 0U : UINT8_MAX;
        Display.drawRectangleSolid(x, 2U, y, y + 3U, brightness);
    }
}

#endif // MODE_BINARYCLOCK
