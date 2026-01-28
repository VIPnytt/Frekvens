#if MODE_SNAKE

#include "modes/SnakeMode.h"

#include "config/constants.h"
#include "extensions/HomeAssistantExtension.h"
#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

#include <Preferences.h>
#include <map>
#include <queue>

void SnakeMode::configure()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_clock");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"clock\":{{value}}}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::enabled_by_default] = false;
        component[HomeAssistantAbbreviations::entity_category] = "config";
        component[HomeAssistantAbbreviations::icon] = "mdi:snake";
        component[HomeAssistantAbbreviations::name] = std::string(name).append(" clock");
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::payload_off] = "false";
        component[HomeAssistantAbbreviations::payload_on] = "true";
        component[HomeAssistantAbbreviations::platform] = "switch";
        component[HomeAssistantAbbreviations::state_off] = "False";
        component[HomeAssistantAbbreviations::state_on] = "True";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.clock}}";
    }
#endif // EXTENSION_HOMEASSISTANT
    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("clock"))
    {
        clock = Storage.getBool("clock");
    }
    Storage.end();
    transmit();
}

void SnakeMode::begin()
{
    Display.clearFrame();
    pending = true;
    stage = 0;
}

void SnakeMode::handle()
{
    if (clock && getLocalTime(&local) && (minute != local.tm_min || hour != local.tm_hour || pending))
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

void SnakeMode::idle()
{
    const uint8_t
        x = random(GRID_COLUMNS),
        y = random(clock ? 5 : 0, GRID_ROWS);
    snake = {{x, y}};
    Display.setPixel(x, y);
    setDot();
    stage = 1;
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
        if (current.y > (clock ? 5 : 0))
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
    if (start.y > (clock ? 5 : 0))
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

void SnakeMode::move()
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
        dot = {(uint8_t)random(GRID_COLUMNS), (uint8_t)random(clock ? 5 : 0, GRID_ROWS)};
    } while (Display.getPixel(dot.x, dot.y));
    Display.setPixel(dot.x, dot.y, random(1, 1 << 8));
}

void SnakeMode::setClock(const bool _clock)
{
    if (_clock != clock)
    {
        clock = _clock;
        if (clock && dot.y < 5)
        {
            setDot();
        }
        else if (!clock)
        {
            Display.drawRectangle(0, 0, GRID_COLUMNS - 1, 4, true, 0);
        }
        Preferences Storage;
        Storage.begin(name);
        Storage.putBool("clock", clock);
        Storage.end();
        pending = true;
        transmit();
    }
}

void SnakeMode::transmit()
{
    JsonDocument doc;
    doc["clock"] = clock;
    Device.transmit(doc, name);
}

void SnakeMode::onReceive(const JsonDocument doc, const char *const source)
{
    // Clock
    if (doc["clock"].is<bool>())
    {
        setClock(doc["clock"].as<bool>());
    }
}

#endif // MODE_SNAKE
