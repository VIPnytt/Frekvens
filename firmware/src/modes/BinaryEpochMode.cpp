#include "modes/BinaryEpochMode.h"
#include "services/DisplayService.h"

void BinaryEpochMode::handle()
{
    time_t _epoch = time(nullptr);
    if (epoch != _epoch)
    {
        epoch = _epoch;
        for (uint8_t i = 0; i < COLUMNS / 2 * ROWS / 4; ++i)
        {
            uint8_t x = COLUMNS - 2 - i % (COLUMNS / 2) * 2;
            uint8_t y = ROWS - 4 - i / (COLUMNS / 2) * 4;
            Display.drawRectangle(x, y, x + 1, y + 3, true, epoch & (1U << i) ? UINT8_MAX : 0);
        }
    }
}
