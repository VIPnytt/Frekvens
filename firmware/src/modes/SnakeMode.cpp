#if MODE_SNAKE

#include "modes/SnakeMode.h"

#include "config/constants.h"                  // NOLINT(misc-include-cleaner)
#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "fonts/MiniFont.h"                    // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h"              // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"

#include <map>
#include <nvs.h>
#include <queue>

static_assert(GRID_COLUMNS >= 16U, __STRING(MODE_SNAKE) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 7U, __STRING(MODE_SNAKE) " is not compatible with this device's display size.");

/**
 * @brief Restores the persisted clock setting and transmits the current configuration.
 */
void SnakeMode::configure()
{
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        uint8_t _clock{0U};
        if (nvs_get_u8(handle, "clock", &_clock) == ESP_OK && static_cast<bool>(_clock))
        {
            clock = std::make_unique<ClockHandler>();
        }
        nvs_close(handle);
    }
    transmit();
}

/**
 * @brief Restores the clock setting, clears the display, and resets the animation stage.
 */
void SnakeMode::begin()
{
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        uint8_t _clock{0U};
        if (nvs_get_u8(handle, "clock", &_clock) == ESP_OK && static_cast<bool>(_clock))
        {
            clock = std::make_unique<ClockHandler>();
        }
        nvs_close(handle);
    }
    Display.fillFrame(0U);
    stage = 0U;
}

/**
 * @brief Updates the clock and advances the snake animation.
 */
void SnakeMode::handle()
{
    if (clock != nullptr)
    {
        clock->handle();
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

/**
 * @brief Places a one-segment snake at a random valid position and starts its movement.
 */
void SnakeMode::idle()
{
    const uint8_t x{static_cast<uint8_t>(random(GRID_COLUMNS))};
    const uint8_t y{static_cast<uint8_t>(random(clock == nullptr ? 0 : 5, GRID_ROWS))};
    snake = {{x, y}};
    Display.setPixel(x, y, UINT8_MAX);
    setTarget();
    stage = 1U;
}

/**
 * @brief Determines the snake's next position toward its target.
 *
 * @return The next unoccupied position, or `std::nullopt` when no adjacent position is available.
 */
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
        if (current.y > (clock == nullptr ? 0U : 5U))
        {
            neighbors.push_back(Pixel{current.x, static_cast<uint8_t>(current.y - 1U)});
        }
        if (current.x < GRID_COLUMNS - 1U)
        {
            neighbors.push_back(Pixel{static_cast<uint8_t>(current.x + 1U), current.y});
        }
        if (current.y < GRID_ROWS - 1U)
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
        Pixel step{target};
        while (from[step] != start)
        {
            step = from[step];
        }
        return step;
    }
    std::vector<Pixel> fallback;
    if (start.y > (clock == nullptr ? 0U : 5U))
    {
        fallback.push_back(Pixel{start.x, static_cast<uint8_t>(start.y - 1U)});
    }
    if (start.x < GRID_COLUMNS - 1U)
    {
        fallback.push_back(Pixel{static_cast<uint8_t>(start.x + 1U), start.y});
    }
    if (start.y < GRID_ROWS - 1U)
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

/**
 * @brief Advances the snake toward its target.
 *
 * Updates the snake's display trail as it moves, selects a new target when the
 * current target is reached, and starts the blinking stage when movement is
 * blocked.
 */
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
                Display.setPixel(target.x, target.y, UINT8_MAX);
                setTarget();
            }
            else
            {
                const uint8_t step{static_cast<uint8_t>(UINT8_MAX / snake.size())};
                for (size_t idx{0U}; idx < snake.size(); ++idx)
                {
                    Display.setPixel(snake[idx].x, snake[idx].y, step * (idx + 1U));
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
        const uint8_t brightness{static_cast<uint8_t>((blinkCount & 0b1U) == 0U ? 0U : UINT8_MAX)};
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
    const uint8_t yMin{static_cast<uint8_t>(clock == nullptr ? 0U : 5U)};
    do // NOLINT(cppcoreguidelines-avoid-do-while)
    {
        target.x = static_cast<uint8_t>(random(GRID_COLUMNS));
        target.y = static_cast<uint8_t>(random(yMin, GRID_ROWS));
    } while (Display.getPixel(target.x, target.y) != 0U);
    Display.setPixel(target.x, target.y, static_cast<uint8_t>(random(1, 0b1U << 8U)));
}

void SnakeMode::setClock(bool _clock)
{
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "clock", static_cast<uint8_t>(_clock)); // NOLINT(readability-implicit-bool-conversion)
        nvs_commit(handle);
        nvs_close(handle);
    }
    if (_clock)
    {
        clock = std::make_unique<ClockHandler>();
        Display.setPixel(target.x, target.y, 0U);
        setTarget();
    }
    else if (clock != nullptr)
    {
        clock->clear();
        clock.reset();
    }
    transmit();
}

/**
 * @brief Transmits the current clock-enabled state.
 */
void SnakeMode::transmit()
{
    JsonDocument doc{};
    doc["clock"].set(clock != nullptr);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

/**
 * @brief Applies the clock setting from an incoming payload.
 *
 * @param payload Incoming JSON payload containing an optional boolean `clock` value.
 */
void SnakeMode::onReceive(JsonObjectConst payload, std::string_view source)
{
    // Clock
    if (payload["clock"].is<bool>())
    {
        setClock(payload["clock"].as<bool>());
    }
}

#if EXTENSION_HOMEASSISTANT
/**
 * @brief Adds the Snake mode clock switch to Home Assistant discovery data.
 *
 * @param discovery Home Assistant discovery document to update.
 * @param topic Base topic used for the switch's state and command messages.
 * @param unique Prefix used to generate the switch's unique identifier.
 */
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
