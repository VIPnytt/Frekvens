#include "config/constants.h"
#include "modes/GameOfLifeMode.h"
#include "services/DisplayService.h"

void GameOfLifeMode::handle()
{
    if (millis() - lastMillis > UINT8_MAX)
    {
        if (active < 10)
        {
            uint8_t
                _x,
                _y;
            for (uint8_t i = 0; i < 8 - active; ++i)
            {
                do
                {
                    _x = random(COLUMNS);
                    _y = random(5, ROWS);
                } while (Display.getPixel(_x, _y));
                Display.setPixel(_x, _y, 1);
            }
        }
        lastMillis = millis();
        active = 0;
        for (uint8_t x = 0; x < COLUMNS; ++x)
        {
            for (uint8_t y = 0; y < ROWS; ++y)
            {
                uint8_t n = 0;
                for (uint8_t _x = x <= 0 ? 0 : x - 1; _x <= x + 1 && _x < COLUMNS; ++_x)
                {
                    for (uint8_t _y = y <= 0 ? 0 : y - 1; _y <= y + 1 && _y < ROWS; ++_y)
                    {
                        if ((_x != x || _y != y) && Display.getPixel(_x, _y))
                        {
                            ++n;
                        }
                    }
                }
                bool p = Display.getPixel(x, y);
                if (p && (n < 2 || n > 3))
                {
                    Display.setPixel(x, y, 0);
                }
                else if (!p && n == 3)
                {
                    Display.setPixel(x, y);
                    ++active;
                }
            }
        }
    }
}
