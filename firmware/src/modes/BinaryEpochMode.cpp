#if MODE_BINARYEPOCH

#include "modes/BinaryEpochMode.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"

static_assert(GRID_COLUMNS >= 16U, __STRING(MODE_BINARYEPOCH) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 16U, __STRING(MODE_BINARYEPOCH) " is not compatible with this device's display size.");

void BinaryEpochMode::handle()
{
    const time_t _epoch{time(nullptr)};
    if (_epoch != epoch)
    {
        epoch = _epoch;
        for (uint8_t i{0U}; i < static_cast<uint8_t>(GRID_COLUMNS * GRID_ROWS / 8U); ++i)
        {
            const uint8_t x{static_cast<uint8_t>(GRID_COLUMNS - 2U - (i % (GRID_COLUMNS / 2U) * 2U))};
            const uint8_t y{static_cast<uint8_t>(GRID_ROWS - 4U - (i / (GRID_COLUMNS / 2U) * 4U))};
            // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
            const uint8_t brightness{static_cast<uint8_t>(
                ((static_cast<std::make_unsigned_t<time_t>>(epoch) >> i) & std::make_unsigned_t<time_t>{1ULL}) == 0U
                    ? 0U
                    : UINT8_MAX)};
            for (uint16_t _y{y}; _y < uint16_t{y} + 4U; ++_y)
            {
                Display.setPixel(static_cast<uint8_t>(x), static_cast<uint8_t>(_y), brightness);
                Display.setPixel(static_cast<uint8_t>(x + 1U), static_cast<uint8_t>(_y), brightness);
            }
        }
    }
}

#endif // MODE_BINARYEPOCH
