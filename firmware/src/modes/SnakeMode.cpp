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

static_assert(GRID_COLUMNS >= 16U, __STRING(MODE_SNAKE) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 7U, __STRING(MODE_SNAKE) " is not compatible with this device's display size.");

void SnakeMode::configure()
{
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        uint8_t _clock{0U};
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
    stage = 0U;
}

void SnakeMode::handle()
{
    if (clock && getLocalTime(&local) && (minute != local.tm_min || hour != local.tm_hour || pending))
    {
        hour = local.tm_hour;
        minute = local.tm_min;
#if CLOCK_12H
        const int hour{(local.tm_hour + 11) % 12 + 1};
#endif // CLOCK_12H
        for (uint8_t x{(GRID_COLUMNS / 2U) - 8U}; x < (GRID_COLUMNS / 2U) + 8U; ++x)
        {
            for (uint8_t y{0U}; y < 5U; ++y)
            {
                Display.setPixel(x, y, 0U);
            }
        }
        const MiniFont font;
        TextHandler(std::to_string(hour / 10), font).draw(GRID_COLUMNS / 2U - 8U, 0U);
        TextHandler(std::to_string(hour % 10), font).draw(GRID_COLUMNS / 2U - 4U, 0U);
        TextHandler(std::to_string(minute / 10), font).draw(GRID_COLUMNS / 2U + 1U, 0U);
        TextHandler(std::to_string(minute % 10), font).draw(GRID_COLUMNS / 2U + 5U, 0U);
        pending = false;
    }
    switch (stage)
    {
    case 1U:
        move();
        break;
    case 2U:
        blink();
        break;
    case 3U:
        clean();
        break;
    default:
        idle();
    }
}

void SnakeMode::idle()
{
    const uint8_t x{static_cast<uint8_t>(random(GRID_COLUMNS))};
    const uint8_t y{static_cast<uint8_t>(random(clock ? 5 : 0, GRID_ROWS))};
    snake = {{x, y}};
    Display.setPixel(x, y);
    setTarget();
    stage = 1U;
}

std::optional<SnakeMode::Pixel> SnakeMode::next() const
{
    Pixel start{snake.back()};
    std::map<Pixel, Pixel> from;
    std::queue<Pixel> frontier;
    frontier.push(start);
    from[start] = start;
    bool pathFound{false};
    while (!frontier.empty())
    {
        Pixel current{frontier.front()};
        frontier.pop();
        if (current == target)
        {
            pathFound = true;
            break;
        }
        std::vector<Pixel> neighbors;
        if (current.x != 0U)
        {
            neighbors.push_back(Pixel{static_cast<uint8_t>(current.x - 1U), current.y});
        }
        if (current.y > (clock ? 5U : 0U))
        {
            neighbors.push_back(Pixel{current.x, static_cast<uint8_t>(current.y - 1U)});
        }
        if (current.x + 1 < GRID_COLUMNS)
        {
            neighbors.push_back(Pixel{static_cast<uint8_t>(current.x + 1U), current.y});
        }
        if (current.y + 1 < GRID_ROWS)
        {
            neighbors.push_back(Pixel{current.x, static_cast<uint8_t>(current.y + 1U)});
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
    if (start.y > (clock ? 5U : 0U))
    {
        fallback.push_back(Pixel{start.x, static_cast<uint8_t>(start.y - 1U)});
    }
    if (start.x + 1U < GRID_COLUMNS)
    {
        fallback.push_back(Pixel{static_cast<uint8_t>(start.x + 1U), start.y});
    }
    if (start.y + 1U < GRID_ROWS)
    {
        fallback.push_back(Pixel{start.x, static_cast<uint8_t>(start.y + 1U)});
    }
    if (start.x != 0U)
    {
        fallback.push_back(Pixel{static_cast<uint8_t>(start.x - 1U), start.y});
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
        std::optional<SnakeMode::Pixel> step{next()};
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
                for (size_t i{0U}; i < snake.size(); ++i)
                {
                    Display.setPixel(snake[i].x, snake[i].y, step * (i + 1U));
                }
                Display.setPixel(snake.front().x, snake.front().y, 0U);
                snake.pop_front();
            }
        }
        else
        {
            lastMillis = millis();
            blinkCount = 0U;
            stage = 2U;
        }
        lastMillis = millis();
    }
}

void SnakeMode::blink()
{
    if (millis() - lastMillis > UINT8_MAX)
    {
        const uint8_t brightness{static_cast<uint8_t>((blinkCount & 1U) == 0U ? 0U : UINT8_MAX)};
        for (const Pixel &pixel : snake)
        {
            Display.setPixel(pixel.x, pixel.y, brightness);
        }
        if (++blinkCount >= 6U)
        {
            stage = 3U;
        }
        lastMillis = millis();
    }
}

void SnakeMode::clean()
{
    if (millis() - lastMillis > INT8_MAX && !snake.empty())
    {
        Display.setPixel(snake.front().x, snake.front().y, 0U);
        snake.pop_front();
        lastMillis = millis();
    }
    else if (snake.empty())
    {
        Display.setPixel(target.x, target.y, 0U);
        stage = 0U;
    }
}

void SnakeMode::setTarget()
{
    const uint8_t yMin{static_cast<uint8_t>(clock ? 5U : 0U)};
    do // NOLINT(cppcoreguidelines-avoid-do-while)
    {
        target.x = random(GRID_COLUMNS);
        target.y = random(yMin, GRID_ROWS);
    } while (Display.getPixel(target.x, target.y) != 0U);
    Display.setPixel(target.x, target.y, random(1, 1U << 8U));
}

void SnakeMode::setClock(bool _clock)
{
    clock = _clock;
    if (clock && target.y < 5U)
    {
        setTarget();
    }
    else if (!clock)
    {
        for (uint8_t x{(GRID_COLUMNS / 2U) - 8U}; x < (GRID_COLUMNS / 2U) + 8U; ++x)
        {
            for (uint8_t y{0U}; y < 5U; ++y)
            {
                Display.setPixel(x, y, 0U);
            }
        }
    }
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "clock", static_cast<uint8_t>(clock)); // NOLINT(readability-implicit-bool-conversion)
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
