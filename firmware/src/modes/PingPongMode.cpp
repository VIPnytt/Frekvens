#include "config/constants.h"
#include "modes/PingPongMode.h"
#include "services/DisplayService.h"

void PingPongMode::wake()
{
    Display.clear();
    paddleT.clear();
    paddleB.clear();
    uint8_t paddleX = random(COLUMNS - 4);
    for (uint8_t _x = 0; _x < 3; ++_x)
    {
        paddleT.push_back(paddleX + _x);
        paddleB.push_back(paddleX + _x);
        Display.setPixel(paddleX + _x, 0);
        Display.setPixel(paddleX + _x, COLUMNS - 1);
    }
    xDec = x = paddleX + 1;
    yDec = y = ROWS - 2;
    deg = random(60, 120); // ±30°
    Display.setPixel(x, y);
    predict();
}

void PingPongMode::handle()
{
    if (yDec <= 1 && deg < 180)
    {
        // Top
        deg = random(225, 315); // ±45°
        predict();
    }
    else if (yDec >= COLUMNS - 2 && deg >= 180)
    {
        // Bottom
        deg = random(45, 135); // ±45°
        predict();
    }
    if (xDec <= 0 || xDec >= COLUMNS - 1)
    {
        // Left/right
        deg = deg >= 180 ? 540 - deg : 180 - deg; // Invert X
        predict();
    }
    Display.setPixel(x, y, 0);
    xDec += cos(deg * DEG_TO_RAD) * speed;
    yDec -= sin(deg * DEG_TO_RAD) * speed;
    x = xDec + .5;
    y = yDec + .5;
    Display.setPixel(x, y);

    if (deg < 180 && targetX - 1 < paddleB.front() && paddleB.front() > 0 && millis() - lastMillis > 200)
    {
        // Top left
        Display.setPixel(paddleB.back(), 0, 0);
        paddleB.pop_back();
        paddleB.push_front(paddleB.front() - 1);
        Display.setPixel(paddleB.front(), 0);
        lastMillis = millis();
    }
    else if (deg < 180 && targetX + 1 > paddleB.back() && paddleB.back() < COLUMNS - 1 && millis() - lastMillis > 200)
    {
        // Top right
        Display.setPixel(paddleB.front(), 0, 0);
        paddleB.pop_front();
        paddleB.push_back(paddleB.back() + 1);
        Display.setPixel(paddleB.back(), 0);
        lastMillis = millis();
    }
    else if (deg >= 180 && targetX - 1 < paddleT.front() && paddleT.front() > 0 && millis() - lastMillis > 200)
    {
        // Bottom left
        Display.setPixel(paddleT.back(), ROWS - 1, 0);
        paddleT.pop_back();
        paddleT.push_front(paddleT.front() - 1);
        Display.setPixel(paddleT.front(), ROWS - 1);
        lastMillis = millis();
    }
    else if (deg >= 180 && targetX + 1 > paddleT.back() && paddleT.back() < COLUMNS - 1 && millis() - lastMillis > 200)
    {
        // Bottom right
        Display.setPixel(paddleT.front(), ROWS - 1, 0);
        paddleT.pop_front();
        paddleT.push_back(paddleT.back() + 1);
        Display.setPixel(paddleT.back(), ROWS - 1);
        lastMillis = millis();
    }
}

void PingPongMode::predict()
{
    double
        _x = xDec,
        _y = yDec;
    do
    {
        _x += cos(deg * DEG_TO_RAD);
        _y -= sin(deg * DEG_TO_RAD);
    } while (_y > 1 && _y + .5 < ROWS - 2);
    targetX = _x + .5;
}
