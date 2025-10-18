#include "config/constants.h"

#if MODE_SNAKE

#include <map>
#include <queue>

#include "modes/SnakeMode.h"
#include "services/DisplayService.h"

void SnakeMode::wake()
{
    Display.clear();
    stage = 0;
}

void SnakeMode::handle()
{
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

void SnakeMode::idle()
{
    const uint8_t
        x = random(COLUMNS),
        y = random(ROWS);
    snake = {{x, y}};
    Display.setPixel(x, y);
    setDot();
    stage = 1;
}

void SnakeMode::move()
{
    if (millis() - lastMillis > 150)
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

void SnakeMode::end()
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

void SnakeMode::clean()
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

void SnakeMode::setDot()
{
    do
    {
        dot = {(uint8_t)random(COLUMNS), (uint8_t)random(ROWS)};
    } while (std::find(snake.begin(), snake.end(), dot) != snake.end());
    Display.setPixel(dot.x, dot.y, random(1, 1 << 8));
}

bool SnakeMode::findPath(Pixel start, Pixel goal, Pixel &next)
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
        if (current.y > 0)
        {
            neighbors.push_back(Pixel{current.x, static_cast<uint8_t>(current.y - 1)});
        }
        if (current.x + 1 < COLUMNS)
        {
            neighbors.push_back(Pixel{static_cast<uint8_t>(current.x + 1), current.y});
        }
        if (current.y + 1 < ROWS)
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
    if (start.x > 0)
    {
        fallback.push_back(Pixel{static_cast<uint8_t>(start.x - 1), start.y});
    }
    if (start.y > 0)
    {
        fallback.push_back(Pixel{start.x, static_cast<uint8_t>(start.y - 1)});
    }
    if (start.x + 1 < COLUMNS)
    {
        fallback.push_back(Pixel{static_cast<uint8_t>(start.x + 1), start.y});
    }
    if (start.y + 1 < ROWS)
    {
        fallback.push_back(Pixel{start.x, static_cast<uint8_t>(start.y + 1)});
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

#endif // MODE_SNAKE
