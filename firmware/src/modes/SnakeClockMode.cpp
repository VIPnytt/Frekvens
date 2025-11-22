#include "config/constants.h"

#if MODE_SNAKECLOCK

#include <map>
#include <queue>

#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "modes/SnakeClockMode.h"
#include "services/DisplayService.h"

void SnakeClockMode::wake()
{
    Display.clearFrame();
    pending = true;
    stage = 0;
}

void SnakeClockMode::handle()
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
    switch (stage)
    {
    case 1:
        move();
        break;
    case 2:
        end();
        break;
    case 3:
        clean();
        break;
    default:
        idle();
    }
}

void SnakeClockMode::idle()
{
    const uint8_t
        x = random(GRID_COLUMNS),
        y = random(5, GRID_ROWS);
    snake = {{x, y}};
    Display.setPixel(x, y);
    setDot();
    stage = 1;
}

void SnakeClockMode::move()
{
    if (millis() - lastMillis > INT8_MAX + snake.size())
    {
        Pixel nextStep;
        if (findPath(snake.back(), dot, nextStep))
        {
            if (nextStep == dot)
            {
                snake.push_back(nextStep);
                Display.setPixel(nextStep.x, nextStep.y, 1);
                setDot();
            }
            else
            {
                snake.push_back(nextStep);
                uint8_t i = 0;
                for (const Pixel &part : snake)
                {
                    Display.setPixel(part.x, part.y, UINT8_MAX / snake.size() * (i + 1));
                    ++i;
                }
                Display.setPixel(snake.front().x, snake.front().y, 0);
                snake.erase(snake.begin());
            }
        }
        else
        {
            lastMillis = millis();
            blink = 0;
            stage = 2;
        }
        lastMillis = millis();
    }
}

void SnakeClockMode::end()
{
    if (millis() - lastMillis > UINT8_MAX)
    {
        for (const Pixel &piece : snake)
        {
            Display.setPixel(piece.x, piece.y, blink % 2 == 0 ? 0 : UINT8_MAX);
        }
        if (++blink >= 6)
        {
            stage = 3;
        }
        lastMillis = millis();
    }
}

void SnakeClockMode::clean()
{
    if (millis() - lastMillis > INT8_MAX && snake.size())
    {
        Display.setPixel(snake.front().x, snake.front().y, 0);
        snake.pop_front();
        lastMillis = millis();
    }
    else if (!snake.size())
    {
        Display.setPixel(dot.x, dot.y, 0);
        stage = 0;
    }
}

void SnakeClockMode::setDot()
{
    do
    {
        dot = {(uint8_t)random(GRID_COLUMNS), (uint8_t)random(5, GRID_ROWS)};
    } while (std::find(snake.begin(), snake.end(), dot) != snake.end());
    Display.setPixel(dot.x, dot.y, random(1, 1 << 8));
}

bool SnakeClockMode::findPath(Pixel start, Pixel goal, Pixel &next)
{
    std::queue<Pixel> frontier;
    std::map<Pixel, Pixel> from;
    frontier.push(start);
    from[start] = start;
    bool pathFound = false;
    while (!frontier.empty())
    {
        Pixel current = frontier.front();
        frontier.pop();

        if (current == goal)
        {
            pathFound = true;
            break;
        }
        std::vector<Pixel> neighbors;
        if (current.x > 0)
        {
            neighbors.push_back(Pixel{static_cast<uint8_t>(current.x - 1), current.y});
        }
        if (current.y > 5)
        {
            neighbors.push_back(Pixel{current.x, static_cast<uint8_t>(current.y - 1)});
        }
        if (current.x + 1 < GRID_COLUMNS)
        {
            neighbors.push_back(Pixel{static_cast<uint8_t>(current.x + 1), current.y});
        }
        if (current.y + 1 < GRID_ROWS)
        {
            neighbors.push_back(Pixel{current.x, static_cast<uint8_t>(current.y + 1)});
        }
        for (const Pixel &neighbor : neighbors)
        {
            if (std::find(snake.begin(), snake.end(), neighbor) == snake.end() && from.find(neighbor) == from.end())
            {
                frontier.push(neighbor);
                from[neighbor] = current;
            }
        }
    }
    if (pathFound)
    {
        Pixel step = goal;
        while (!(from[step] == start))
        {
            step = from[step];
        }
        next = step;
        return true;
    }
    std::vector<Pixel> fallback;
    if (start.y > 5)
    {
        fallback.push_back(Pixel{start.x, static_cast<uint8_t>(start.y - 1)});
    }
    if (start.x + 1 < GRID_COLUMNS)
    {
        fallback.push_back(Pixel{static_cast<uint8_t>(start.x + 1), start.y});
    }
    if (start.y + 1 < GRID_ROWS)
    {
        fallback.push_back(Pixel{start.x, static_cast<uint8_t>(start.y + 1)});
    }
    if (start.x > 0)
    {
        fallback.push_back(Pixel{static_cast<uint8_t>(start.x - 1), start.y});
    }
    for (const Pixel &option : fallback)
    {
        if (std::find(snake.begin(), snake.end(), option) == snake.end())
        {
            next = option;
            return true;
        }
    }
    return false;
}

#endif // MODE_SNAKECLOCK
