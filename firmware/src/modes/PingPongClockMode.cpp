#include "config/constants.h"
#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "modes/PingPongClockMode.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void PingPongClockMode::wake()
{
    hour = 0;
    minute = 0;
    Display.clear();
    paddleL.clear();
    paddleR.clear();
    uint8_t paddleY = random(5, ROWS - 4);
    for (uint8_t _y = 0; _y < 3; ++_y)
    {
        paddleL.push_back(paddleY + _y);
        paddleR.push_back(paddleY + _y);
        Display.setPixel(0, paddleY + _y);
        Display.setPixel(ROWS - 1, paddleY + _y);
    }
    xDec = x = COLUMNS - 2;
    yDec = y = paddleY + 1;
    deg = random(150, 210); // ±30°
    Display.setPixel(x, y);
    predict();
}

void PingPongClockMode::handle()
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
    if (xDec <= 1 && deg >= 90 && deg < 270)
    {
        // Left
        deg = (random(45, 135) + 270) % 360; // ±45°
        predict();
    }
    else if (xDec >= COLUMNS - 2 && (deg < 90 || deg >= 270))
    {
        // Right
        deg = random(135, 225); // ±45°
        predict();
    }
    if (yDec <= 5 || yDec >= ROWS - 1)
    {
        // Top/bottom
        deg = 360 - deg; // Invert Y
        predict();
    }
    Display.setPixel(x, y, 0);
    xDec += cos(deg * DEG_TO_RAD) * speed;
    yDec -= sin(deg * DEG_TO_RAD) * speed;
    x = xDec + .5;
    y = yDec + .5;
    Display.setPixel(x, y);

    if (deg >= 90 && deg < 270 && targetY - 1 < paddleL.front() && paddleL.front() > 5 && millis() - lastMillis > UINT8_MAX)
    {
        // Left up
        Display.setPixel(0, paddleL.back(), 0);
        paddleL.pop_back();
        paddleL.push_front(paddleL.front() - 1);
        Display.setPixel(0, paddleL.front());
        lastMillis = millis();
    }
    else if (deg >= 90 && deg < 270 && targetY + 1 > paddleL.back() && paddleL.back() < ROWS - 1 && millis() - lastMillis > UINT8_MAX)
    {
        // Left down
        Display.setPixel(0, paddleL.front(), 0);
        paddleL.pop_front();
        paddleL.push_back(paddleL.back() + 1);
        Display.setPixel(0, paddleL.back());
        lastMillis = millis();
    }
    else if ((deg < 90 || deg >= 270) && targetY - 1 < paddleR.front() && paddleR.front() > 5 && millis() - lastMillis > UINT8_MAX)
    {
        // Right up
        Display.setPixel(ROWS - 1, paddleR.back(), 0);
        paddleR.pop_back();
        paddleR.push_front(paddleR.front() - 1);
        Display.setPixel(ROWS - 1, paddleR.front());
        lastMillis = millis();
    }
    else if ((deg < 90 || deg >= 270) && targetY + 1 > paddleR.back() && paddleR.back() < ROWS - 1 && millis() - lastMillis > UINT8_MAX)
    {
        // Right down
        Display.setPixel(ROWS - 1, paddleR.front(), 0);
        paddleR.pop_front();
        paddleR.push_back(paddleR.back() + 1);
        Display.setPixel(ROWS - 1, paddleR.back());
        lastMillis = millis();
    }
}

void PingPongClockMode::predict()
{
    double
        _x = xDec,
        _y = yDec;
    do
    {
        _x += cos(deg * DEG_TO_RAD);
        _y -= sin(deg * DEG_TO_RAD);
    } while (_x > 1 && _x + .5 < COLUMNS - 2);
    targetY = _y + .5;
}
