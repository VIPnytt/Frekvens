#if MODE_SNAKE

#include "modes/SnakeMode.h"

#include "config/constants.h"     // NOLINT(misc-include-cleaner)
#include "fonts/MiniFont.h"       // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

#include <map>
#include <nvs.h>
#include <queue>

void SnakeMode::configure()
{
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        uint8_t _clock{0};
        if (nvs_get_u8(handle, "clock", &_clock) == ESP_OK)
        {
            clock = static_cast<bool>(_clock);
        }
        nvs_close(handle);
    }
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
        const MiniFont font;
        TextHandler(std::to_string(hour / 10), font).draw(GRID_COLUMNS / 2 - 8, 0);
        TextHandler(std::to_string(hour % 10), font).draw(GRID_COLUMNS / 2 - 4, 0);
        TextHandler(std::to_string(minute / 10), font).draw(GRID_COLUMNS / 2 + 1, 0);
        TextHandler(std::to_string(minute % 10), font).draw(GRID_COLUMNS / 2 + 5, 0);
        pending = false;
    }
    switch (stage)
    {
    case 1:
        move();
        break;
    case 2:
        blink();
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
    const uint8_t x = random(GRID_COLUMNS);
    const uint8_t y = random(clock ? 5 : 0, GRID_ROWS);
    snake = {{x, y}};
    Display.setPixel(x, y);
    setTarget();
    stage = 1;
}

std::optional<SnakeMode::Pixel> SnakeMode::next() const
{
    Pixel start = snake.back();
    std::map<Pixel, Pixel> from;
    std::queue<Pixel> frontier;
    frontier.push(start);
    from[start] = start;
    bool pathFound = false;
    while (!frontier.empty())
    {
        Pixel current = frontier.front();
        frontier.pop();
        if (current == target)
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
        Pixel step{target}; // NOLINT(misc-const-correctness)
        while (from.at(step) != start)
        {
            step = from.at(step);
        }
        return step;
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
            return option;
        }
    }
    return std::nullopt;
}

void SnakeMode::move()
{
    if (millis() - lastMillis > INT8_MAX + snake.size())
    {
        std::optional<SnakeMode::Pixel> step = next();
        if (step.has_value())
        {
            snake.push_back(step.value());
            if (snake.back() == target)
            {
                Display.setPixel(target.x, target.y);
                setTarget();
            }
            else
            {
                // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
                const uint8_t step{static_cast<uint8_t>(UINT8_MAX / snake.size())};
                for (std::size_t i = 0; i < snake.size(); ++i)
                {
                    Display.setPixel(snake[i].x, snake[i].y, step * (i + 1));
                }
                Display.setPixel(snake.front().x, snake.front().y, 0);
                snake.pop_front();
            }
        }
        else
        {
            lastMillis = millis();
            blinkCount = 0;
            stage = 2;
        }
        lastMillis = millis();
    }
}

void SnakeMode::blink()
{
    if (millis() - lastMillis > UINT8_MAX)
    {
        const uint8_t brightness = (blinkCount & 1U) == 0 ? 0 : UINT8_MAX;
        for (const Pixel &pixel : snake)
        {
            Display.setPixel(pixel.x, pixel.y, brightness);
        }
        if (++blinkCount >= 6)
        {
            stage = 3;
        }
        lastMillis = millis();
    }
}

void SnakeMode::clean()
{
    if (millis() - lastMillis > INT8_MAX && !snake.empty())
    {
        Display.setPixel(snake.front().x, snake.front().y, 0);
        snake.pop_front();
        lastMillis = millis();
    }
    else if (snake.empty())
    {
        Display.setPixel(target.x, target.y, 0);
        stage = 0;
    }
}

void SnakeMode::setTarget()
{
    const uint8_t yMin = clock ? 5U : 0U;
    do // NOLINT(cppcoreguidelines-avoid-do-while)
    {
        target.x = random(GRID_COLUMNS);
        target.y = random(yMin, GRID_ROWS);
    } while (Display.getPixel(target.x, target.y) != 0);
    Display.setPixel(target.x, target.y, random(1, 1U << 8U));
}

void SnakeMode::setClock(bool _clock)
{
    clock = _clock;
    if (clock && target.y < 5)
    {
        setTarget();
    }
    else if (!clock)
    {
        Display.drawRectangle(0, 0, GRID_COLUMNS - 1, 4, true, 0);
    }
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "clock", static_cast<uint8_t>(clock));
        nvs_commit(handle);
        nvs_close(handle);
    }
    pending = true;
    transmit();
}

void SnakeMode::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["clock"].set(clock);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void SnakeMode::onReceive(JsonObjectConst payload,
                          std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Clock
    if (payload["clock"].is<bool>())
    {
        setClock(payload["clock"].as<bool>());
    }
}

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void SnakeMode::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_clock")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"clock":{{value}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:snake");
        component[HomeAssistantAbbreviations::name].set(std::string(name).append(" clock"));
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::payload_off].set("false");
        component[HomeAssistantAbbreviations::payload_on].set("true");
        component[HomeAssistantAbbreviations::platform].set("switch");
        component[HomeAssistantAbbreviations::state_off].set("False");
        component[HomeAssistantAbbreviations::state_on].set("True");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.clock}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // MODE_SNAKE
