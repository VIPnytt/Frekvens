#if MODE_PINGPONGCLOCK

#include "modes/PingPongClockMode.h"

#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void PingPongClockMode::begin()
{
    pending = true;
    Display.clearFrame();
    paddleL.clear();
    paddleR.clear();
    const uint8_t paddleY = random(5, GRID_ROWS - 3);
    for (uint8_t _y = 0; _y < 3; ++_y)
    {
        paddleL.push_back(paddleY + _y);
        paddleR.push_back(paddleY + _y);
        Display.setPixel(0, paddleY + _y);
        Display.setPixel(GRID_ROWS - 1, paddleY + _y);
    }
    xDec = x = GRID_COLUMNS - 2;
    yDec = y = paddleY + 1;
    deg = random(150, 211); // ±30°
    Display.setPixel(x, y);
}

void PingPongClockMode::handle()
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
    if (xDec <= 1 && deg >= 90 && deg < 270)
    {
        // Left
        deg = (random(45, 136) + 270) % 360; // ±45°
    }
    else if (xDec >= GRID_COLUMNS - 2 && (deg < 90 || deg >= 270))
    {
        // Right
        deg = random(135, 226); // ±45°
    }
    if (yDec <= 5 || yDec >= GRID_ROWS - 1)
    {
        // Top/bottom
        deg = 360 - deg; // Invert Y
    }
    Display.setPixel(x, y, 0);
    xDec += cos(deg * DEG_TO_RAD) * speed;
    yDec -= sin(deg * DEG_TO_RAD) * speed;
    x = xDec + .5f;
    y = yDec + .5f;
    Display.setPixel(x, y);
    const float
        lRad = atanf((xDec - 1) / abs(paddleL[1] - yDec)),
        rRad = atanf((GRID_COLUMNS - 2 - xDec) / abs(paddleR[1] - yDec));
    if (yDec > paddleL.back() && lRad < 1 && paddleL.back() < GRID_ROWS - 1)
    {
        // Left down
        Display.setPixel(0, paddleL.front(), 0);
        paddleL.pop_front();
        paddleL.push_back(paddleL.back() + 1);
        Display.setPixel(0, paddleL.back());
    }
    else if (yDec < paddleL.front() && lRad < 1 && paddleL.front() > 5)
    {
        // Left up
        Display.setPixel(0, paddleL.back(), 0);
        paddleL.pop_back();
        paddleL.push_front(paddleL.front() - 1);
        Display.setPixel(0, paddleL.front());
    }
    else if (yDec > paddleR.back() && rRad < 1 && paddleR.back() < GRID_ROWS - 1)
    {
        // Right down
        Display.setPixel(GRID_COLUMNS - 1, paddleR.front(), 0);
        paddleR.pop_front();
        paddleR.push_back(paddleR.back() + 1);
        Display.setPixel(GRID_COLUMNS - 1, paddleR.back());
    }
    else if (yDec < paddleR.front() && rRad < 1 && paddleR.front() > 5)
    {
        // Right up
        Display.setPixel(GRID_COLUMNS - 1, paddleR.back(), 0);
        paddleR.pop_back();
        paddleR.push_front(paddleR.front() - 1);
        Display.setPixel(GRID_COLUMNS - 1, paddleR.front());
    }
}

#endif // MODE_PINGPONGCLOCK
