#include "config/constants.h"
#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "modes/BreakoutClockMode.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void BreakoutClockMode::wake()
{
    Display.drawRectangle(0, 0, COLUMNS - 1, 4);
    paddle.clear();
    uint8_t paddleX = random(COLUMNS - 4);
    for (uint8_t _x = 0; _x < 3; ++_x)
    {
        paddle.push_back(paddleX + _x);
        Display.setPixel(paddleX + _x, ROWS - 1);
    }
    deg = random(60, 120); // ±30°
    xDec = x = paddleX + 1;
    yDec = y = ROWS - 2;
    Display.setPixel(x, y);
}

void BreakoutClockMode::handle()
{
    uint8_t
        nextX = xDec + cos(deg * DEG_TO_RAD) * speed + .5,
        nextY = yDec - sin(deg * DEG_TO_RAD) * speed + .5;
    if (y <= 0 && deg < 180)
    {
        // Top
        deg = 360 - deg; // Invert Y
    }
    if ((x <= 0 && deg >= 90 && deg < 270) || (x >= COLUMNS - 1 && (deg < 90 || deg > 270)))
    {
        // Wall
        deg = deg >= 180 ? 540 - deg : 180 - deg; // Invert X
    }
    if (nextY >= ROWS - 1 && deg > 180)
    {
        // Bottom
        deg = random(30, 150); // ±60°
        if (getLocalTime(&local) && (minute != local.tm_min || hour != local.tm_hour))
        {
            hour = local.tm_hour;
            minute = local.tm_min;
            TextHandler((String)(hour / 10), FontMini).draw(0, 0);
            TextHandler((String)(hour % 10), FontMini).draw(4, 0);
            TextHandler((String)(minute / 10), FontMini).draw(9, 0);
            TextHandler((String)(minute % 10), FontMini).draw(13, 0);
        }
    }
    else if ((nextX != x || nextY != y) && Display.getPixel(nextX, nextY))
    {
        // Brick
        Display.setPixel(nextX, nextY, 0);
        if (nextX == x)
        {
            deg = 360 - deg; // Invert Y
        }
        else if (nextY = y)
        {
            deg = deg >= 180 ? 540 - deg : 180 - deg; // Invert X
        }
    }
    Display.setPixel(x, y, 0);
    xDec += cos(deg * DEG_TO_RAD) * speed;
    yDec -= sin(deg * DEG_TO_RAD) * speed;
    x = xDec + .5;
    y = yDec + .5;
    Display.setPixel(x, y);

    double angle = atan((ROWS - 2 - yDec) / abs(paddle[1] - xDec)) * RAD_TO_DEG;
    if (xDec > paddle.back() && angle < 67.5 && paddle.back() < COLUMNS - 1)
    {
        // Right
        Display.setPixel(paddle.front(), COLUMNS - 1, 0);
        paddle.pop_front();
        paddle.push_back(paddle.back() + 1);
        Display.setPixel(paddle.back(), COLUMNS - 1);
    }
    else if (xDec < paddle.front() && angle < 67.5 && paddle.front() > 0)
    {
        // Left
        Display.setPixel(paddle.back(), COLUMNS - 1, 0);
        paddle.pop_back();
        paddle.push_front(paddle.front() - 1);
        Display.setPixel(paddle.front(), COLUMNS - 1);
    }
}
