#if MODE_BREAKOUTCLOCK

#include "modes/BreakoutClockMode.h"

#include "fonts/MiniFont.h"       // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

static_assert(GRID_COLUMNS >= 16U, __STRING(MODE_BREAKOUTCLOCK) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 10U, __STRING(MODE_BREAKOUTCLOCK) " is not compatible with this device's display size.");

/**
 * @brief Initializes the Breakout game state and display.
 *
 * Fills the upper playfield with bricks, positions and draws the paddle,
 * and places the ball above the paddle with a random launch angle.
 */
void BreakoutClockMode::begin()
{
    Display.fillRows(0U, GRID_ROWS - 4U, UINT8_MAX);
    paddle.clear();
    const uint8_t paddleX{static_cast<uint8_t>(random(GRID_COLUMNS - 4U))};
    paddle.push_back(paddleX);
    paddle.push_back(paddleX + 1U);
    paddle.push_back(paddleX + 2U);
    Display.drawLineHorizontal(paddleX, 3U, GRID_ROWS - 1U, UINT8_MAX);
    deg = static_cast<uint16_t>(random(60, 121)); // ±30°
    xDec = x = paddleX + 1U;
    yDec = y = GRID_ROWS - 2U;
    Display.setPixel(x, y, UINT8_MAX);
}

/**
 * @brief Updates the game clock, ball position, collisions, and paddle movement.
 *
 * Pauses the ball's launch when microphone-triggered launching is enabled and
 * awaiting activation.
 */
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
        deg = static_cast<uint16_t>(random(30, 151)); // ±60°
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
    Display.setPixel(x, y, UINT8_MAX);
    const float rad{atanf((GRID_ROWS - 2U - yDec) / abs(paddle[1U] - xDec))};
    if (xDec < paddle.front() && rad < 1.0F && paddle.front() != 0U)
    {
        // Left
        Display.setPixel(paddle.back(), GRID_ROWS - 1U, 0U);
        paddle.pop_back();
        paddle.push_front(paddle.front() - 1U);
        Display.setPixel(paddle.front(), GRID_ROWS - 1U, UINT8_MAX);
    }
    else if (xDec > paddle.back() && rad < 1.0F && paddle.back() < GRID_COLUMNS - 1U)
    {
        // Right
        Display.setPixel(paddle.front(), GRID_ROWS - 1U, 0U);
        paddle.pop_front();
        paddle.push_back(paddle.back() + 1U);
        Display.setPixel(paddle.back(), GRID_ROWS - 1U, UINT8_MAX);
    }
}

#endif // MODE_BREAKOUTCLOCK
