#if MODE_GAMEOFLIFE

#include "config/constants.h"
#include "modes/GameOfLifeMode.h"
#include "services/DisplayService.h"

void GameOfLifeMode::handle()
{
    if (millis() - lastMillis > UINT8_MAX)
    {
        bool seeds[GRID_COLUMNS * GRID_ROWS] = {false};
        for (uint8_t i = active; i < GRID_COLUMNS * GRID_ROWS / (1 << 4); ++i)
        {
            seeds[random(1, GRID_COLUMNS - 1) + random(1, GRID_ROWS - 1) * GRID_COLUMNS] = true;
        }
        lastMillis = millis();
        active = 0;
        for (uint8_t x = 0; x < GRID_COLUMNS; ++x)
        {
            for (uint8_t y = 0; y < GRID_ROWS; ++y)
            {
                uint8_t n = 0;
                for (uint8_t _x = x <= 0 ? 0 : x - 1; _x <= x + 1 && _x < GRID_COLUMNS; ++_x)
                {
                    for (uint8_t _y = y <= 0 ? 0 : y - 1; _y <= y + 1 && _y < GRID_ROWS; ++_y)
                    {
                        if ((_x != x || _y != y) && (seeds[_x + _y * GRID_COLUMNS] || Display.getPixel(_x, _y)))
                        {
                            ++n;
                        }
                    }
                }
                const bool lit = seeds[x + y * GRID_COLUMNS] || Display.getPixel(x, y);
                if (lit && (n < 2 || n > 3))
                {
                    Display.setPixel(x, y, 0);
                }
                else if (!lit && n == 3)
                {
                    Display.setPixel(x, y);
                    ++active;
                }
            }
        }
    }
}

#endif // MODE_GAMEOFLIFE
