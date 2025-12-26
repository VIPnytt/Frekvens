#include "config/constants.h"

#if MODE_PINGPONG

#include "modes/PingPongMode.h"
#include "services/DisplayService.h"

void PingPongMode::begin()
{
    Display.clearFrame();
    paddleB.clear();
    paddleT.clear();
    const uint8_t paddleX = random(GRID_COLUMNS - 3);
    for (uint8_t _x = 0; _x < 3; ++_x)
    {
        paddleB.push_back(paddleX + _x);
        paddleT.push_back(paddleX + _x);
        Display.setPixel(paddleX + _x, 0);
        Display.setPixel(paddleX + _x, GRID_COLUMNS - 1);
    }
    xDec = x = paddleX + 1;
    yDec = y = GRID_ROWS - 2;
    deg = random(60, 121); // ±30°
    Display.setPixel(x, y);
}

void PingPongMode::handle()
{
    if (yDec <= 1 && deg < 180)
    {
        // Top
        deg = random(225, 316); // ±45°
    }
    else if (yDec >= GRID_COLUMNS - 2 && deg >= 180)
    {
        // Bottom
        deg = random(45, 136); // ±45°
    }
    if (xDec <= 0 || xDec >= GRID_COLUMNS - 1)
    {
        // Left/right
        deg = deg >= 180 ? 540 - deg : 180 - deg; // Invert X
    }
    Display.setPixel(x, y, 0);
    xDec += cos(deg * DEG_TO_RAD) * speed;
    yDec -= sin(deg * DEG_TO_RAD) * speed;
    x = xDec + .5f;
    y = yDec + .5f;
    Display.setPixel(x, y);
    const float
        bRad = atanf((GRID_ROWS - 2 - yDec) / abs(paddleB[1] - xDec)),
        tRad = atanf((yDec - 1) / abs(paddleT[1] - xDec));
    if (xDec < paddleB.front() && bRad < 1 && paddleB.front() > 0)
    {
        // Bottom left
        Display.setPixel(paddleB.back(), GRID_ROWS - 1, 0);
        paddleB.pop_back();
        paddleB.push_front(paddleB.front() - 1);
        Display.setPixel(paddleB.front(), GRID_ROWS - 1);
    }
    else if (xDec > paddleB.back() && bRad < 1 && paddleB.back() < GRID_COLUMNS - 1)
    {
        // Bottom right
        Display.setPixel(paddleB.front(), GRID_ROWS - 1, 0);
        paddleB.pop_front();
        paddleB.push_back(paddleB.back() + 1);
        Display.setPixel(paddleB.back(), GRID_ROWS - 1);
    }
    else if (xDec < paddleT.front() && tRad < 1 && paddleT.front() > 0)
    {
        // Top left
        Display.setPixel(paddleT.back(), 0, 0);
        paddleT.pop_back();
        paddleT.push_front(paddleT.front() - 1);
        Display.setPixel(paddleT.front(), 0);
    }
    else if (xDec > paddleT.back() && tRad < 1 && paddleT.back() < GRID_COLUMNS - 1)
    {
        // Top right
        Display.setPixel(paddleT.front(), 0, 0);
        paddleT.pop_front();
        paddleT.push_back(paddleT.back() + 1);
        Display.setPixel(paddleT.back(), 0);
    }
}

#endif // MODE_PINGPONG
