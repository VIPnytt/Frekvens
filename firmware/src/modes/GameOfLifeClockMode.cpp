#include "config/constants.h"
#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "modes/GameOfLifeClockMode.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void GameOfLifeClockMode::wake()
{
    hour = 0;
    minute = 0;
}

void GameOfLifeClockMode::handle()
{
    if (millis() - lastMillis > UINT8_MAX)
    {
        if (getLocalTime(&local) && (minute != local.tm_min || hour != local.tm_hour))
        {
            hour = local.tm_hour;
            minute = local.tm_min;
            Display.drawRectangle(0, 0, COLUMNS - 1, 4, true, 0);
            TextHandler((String)(hour / 10), FontMini).draw(0, 0);
            TextHandler((String)(hour % 10), FontMini).draw(4, 0);
            TextHandler((String)(minute / 10), FontMini).draw(9, 0);
            TextHandler((String)(minute % 10), FontMini).draw(13, 0);
        }
        else if (active < 10)
        {
            uint8_t
                _x,
                _y;
            for (uint8_t i = 0; i < 8 - active; ++i)
            {
                do
                {
                    _x = random(COLUMNS);
                    _y = random(6, ROWS);
                } while (Display.getPixel(_x, _y));
                Display.setPixel(_x, _y, 1);
            }
        }
        lastMillis = millis();
        active = 0;
        for (uint8_t x = 0; x < COLUMNS; ++x)
        {
            for (uint8_t y = 5; y < ROWS; ++y)
            {
                uint8_t n = 0;
                for (uint8_t _x = x <= 0 ? 0 : x - 1; _x <= x + 1 && _x < COLUMNS; ++_x)
                {
                    for (uint8_t _y = y <= 5 ? 5 : y - 1; _y <= y + 1 && _y < ROWS; ++_y)
                    {
                        if ((_x != x || _y != y) && Display.getPixel(_x, _y))
                        {
                            ++n;
                        }
                    }
                }
                bool lit = Display.getPixel(x, y);
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
