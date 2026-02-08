#if MODE_BINARYEPOCH

#include "modes/BinaryEpochMode.h"

#include "config/constants.h"
#include "services/DisplayService.h"

void BinaryEpochMode::handle()
{
    time_t _epoch = time(nullptr);
    if (epoch != _epoch)
    {
        epoch = _epoch;
        for (uint8_t i = 0; i < GRID_COLUMNS * GRID_ROWS / 8; ++i)
        {
            const uint8_t x = GRID_COLUMNS - 2 - i % (GRID_COLUMNS / 2) * 2,
                          y = GRID_ROWS - 4 - i / (GRID_COLUMNS / 2) * 4;
            Display.drawRectangle(x, y, x + 1, y + 3, true, epoch & (1 << i) ? UINT8_MAX : 0);
        }
    }
}

#endif // MODE_BINARYEPOCH
