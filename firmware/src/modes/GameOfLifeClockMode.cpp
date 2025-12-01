#if MODE_GAMEOFLIFECLOCK

#include "config/constants.h"
#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "modes/GameOfLifeClockMode.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void GameOfLifeClockMode::begin()
{
    pending = true;
}

void GameOfLifeClockMode::handle()
{
    if (millis() - lastMillis > UINT8_MAX)
    {
        if (getLocalTime(&local) && (minute != local.tm_min || hour != local.tm_hour || pending))
        {
            hour = local.tm_hour;
            minute = local.tm_min;
            Display.drawRectangle(0, 0, GRID_COLUMNS - 1, 4, true, 0);
            TextHandler(std::to_string(hour / 10), FontMini).draw(GRID_COLUMNS / 2 - 8, 0);
            TextHandler(std::to_string(hour % 10), FontMini).draw(GRID_COLUMNS / 2 - 4, 0);
            TextHandler(std::to_string(minute / 10), FontMini).draw(GRID_COLUMNS / 2 + 1, 0);
            TextHandler(std::to_string(minute % 10), FontMini).draw(GRID_COLUMNS / 2 + 5, 0);
            pending = false;
        }
        bool seeds[GRID_COLUMNS * (GRID_ROWS - 5)] = {false};
        for (uint8_t i = active; i < GRID_COLUMNS * (GRID_ROWS - 5) / (1 << 4); ++i)
        {
            seeds[random(1, GRID_COLUMNS - 1) + random(6, GRID_ROWS - 1) * (GRID_COLUMNS - 5)] = true;
        }
        lastMillis = millis();
        active = 0;
        for (uint8_t x = 0; x < GRID_COLUMNS; ++x)
        {
            for (uint8_t y = 5; y < GRID_ROWS; ++y)
            {
                uint8_t n = 0;
                for (uint8_t _x = x <= 0 ? 0 : x - 1; _x <= x + 1 && _x < GRID_COLUMNS; ++_x)
                {
                    for (uint8_t _y = y <= 5 ? 5 : y - 1; _y <= y + 1 && _y < GRID_ROWS; ++_y)
                    {
                        if ((_x != x || _y != y) && (seeds[_x + _y * (GRID_COLUMNS - 5)] || Display.getPixel(_x, _y)))
                        {
                            ++n;
                        }
                    }
                }
                const bool lit = seeds[x + y * (GRID_COLUMNS - 5)] || Display.getPixel(x, y);
                if (lit && (n < 2 || n > 3))
                {
                    Display.setPixel(x, y, 0);
                }
                else if (!lit && n == 3)
                {
                    Display.setPixel(x, y, INT8_MAX);
                    ++active;
                }
            }
        }
    }
}

#endif // MODE_GAMEOFLIFECLOCK
