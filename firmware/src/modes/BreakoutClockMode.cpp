#if MODE_BREAKOUTCLOCK

#include "modes/BreakoutClockMode.h"

#include "fonts/MiniFont.h"       // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS >= 16U, __STRING(MODE_BREAKOUTCLOCK) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 10U, __STRING(MODE_BREAKOUTCLOCK) " is not compatible with this device's display size.");

void BreakoutClockMode::begin()
{
    for (uint8_t _x{0U}; _x < GRID_COLUMNS; ++_x)
    {
        for (uint8_t _y{0U}; _y < GRID_ROWS - 4U; ++_y)
        {
            Display.setPixel(_x, _y);
        }
    }
    paddle.clear();
    const uint8_t paddleX{static_cast<uint8_t>(random(GRID_COLUMNS - 4U))};
    for (uint8_t _x{0U}; _x < 3U; ++_x)
    {
        paddle.push_back(paddleX + _x);
        Display.setPixel(paddleX + _x, GRID_ROWS - 1U);
    }
    deg = random(60, 121); // ±30°
    xDec = x = paddleX + 1U;
    yDec = y = GRID_ROWS - 2U;
    Display.setPixel(x, y);
}

void BreakoutClockMode::handle()
{
    clock.handle();
#if EXTENSION_MICROPHONE
    if (y == GRID_ROWS - 2U && deg < 180U && !Extensions.Microphone().isTriggered())
    {
        return;
    }
#endif // EXTENSION_MICROPHONE
    const uint8_t nextX{
        static_cast<uint8_t>(lroundf(xDec + (cosf(static_cast<float>(deg) * static_cast<float>(DEG_TO_RAD)) * speed)))};
    // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
    const uint8_t nextY{static_cast<uint8_t>(
        std::lroundf(yDec - (sinf(static_cast<float>(deg) * static_cast<float>(DEG_TO_RAD)) * speed)))};
    if (y == 0U && deg < 180U)
    {
        // Top
        deg = 360U - deg; // Invert Y
    }
    if ((x == 0U && deg >= 90U && deg < 270U) || (x >= GRID_COLUMNS - 1U && (deg < 90U || deg > 270U)))
    {
        // Wall
        deg = deg >= 180U ? 540U - deg : 180U - deg; // Invert X
    }
    if (nextY >= GRID_ROWS - 1U && deg > 180U)
    {
        // Bottom
        deg = random(30, 151); // ±60°
    }
    else if ((nextX != x || nextY != y) && Display.getPixel(nextX, nextY) != 0U)
    {
        // Brick
        Display.setPixel(nextX, nextY, 0U);
        if (nextX == x)
        {
            deg = 360U - deg; // Invert Y
        }
        else if (nextY == y)
        {
            deg = deg >= 180U ? 540U - deg : 180U - deg; // Invert X
        }
    }
    Display.setPixel(x, y, 0U);
    xDec += cosf(static_cast<float>(deg) * static_cast<float>(DEG_TO_RAD)) * speed;
    yDec -= sinf(static_cast<float>(deg) * static_cast<float>(DEG_TO_RAD)) * speed;
    x = lroundf(xDec);
    y = lroundf(yDec);
    Display.setPixel(x, y);
    const float rad{atanf((GRID_ROWS - 2U - yDec) / abs(paddle[1U] - xDec))};
    if (xDec < paddle.front() && rad < 1.0F && paddle.front() != 0U) // NOLINT(bugprone-branch-clone)
    {
        // Left
        Display.setPixel(paddle.back(), GRID_ROWS - 1U, 0U);
        paddle.pop_back();
        paddle.push_front(paddle.front() - 1U);
        Display.setPixel(paddle.front(), GRID_ROWS - 1U);
    }
    else if (xDec > paddle.back() && rad < 1.0F && paddle.back() < GRID_COLUMNS - 1U) // NOLINT(bugprone-branch-clone)
    {
        // Right
        Display.setPixel(paddle.front(), GRID_ROWS - 1U, 0U);
        paddle.pop_front();
        paddle.push_back(paddle.back() + 1U);
        Display.setPixel(paddle.back(), GRID_ROWS - 1U);
    }
}

#endif // MODE_BREAKOUTCLOCK
