#if MODE_SNAKE

#include "modes/SnakeMode.h"

#include "config/constants.h"                  // NOLINT(misc-include-cleaner)
#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "fonts/MiniFont.h"                    // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h"              // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"

#include <algorithm>
#include <array>
#include <nvs.h>

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
    stage = Stage::READY;
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
    case Stage::READY:
        idle();
        break;
    case Stage::MOVE:
        move();
        break;
    case Stage::DEATH:
        blink();
        break;
    case Stage::REMOVE:
        clean();
        break;
    }
}

/**
 * @brief Places a one-segment snake at a random valid position and starts its movement.
 */
void SnakeMode::idle()
{
    snake = {static_cast<size_t>((random(clock == nullptr ? 0 : 5, GRID_ROWS) * GRID_COLUMNS) + random(GRID_COLUMNS))};
    Display.setPixel(snake.front(), static_cast<uint8_t>(random(1, 0b1U << 8U)));
    target = static_cast<size_t>(random(static_cast<long>(clock == nullptr ? 0U : 5U * GRID_COLUMNS),
                                        static_cast<long>(GRID_COLUMNS * GRID_ROWS)));
    stage = Stage::MOVE;
}

/**
 * @brief Determines the snake's next position toward its target.
 *
 * @return The next unoccupied position, or `std::nullopt` when no adjacent position is available.
 */
std::optional<size_t> SnakeMode::findStepPath() const
{
    const uint8_t yMin{static_cast<uint8_t>(clock == nullptr ? 0U : 5U)};
    std::array<size_t, GRID_COLUMNS * GRID_ROWS> from{};
    std::array<size_t, GRID_COLUMNS * GRID_ROWS> frontier{};
    std::array<bool, GRID_COLUMNS * GRID_ROWS> visited{};
    size_t frontierHead{0U};
    size_t frontierTail{0U};
    const size_t start{snake.back()};
    frontier[frontierTail++] = start;
    from[start] = start;
    visited[start] = true;
    while (frontierHead < frontierTail)
    {
        const size_t current{frontier[frontierHead++]};
        if (current == target)
        {
            size_t step{target};
            while (from[step] != start)
            {
                step = from[step];
            }
            return step; // NOLINT(bugprone-narrowing-conversions)
        }
        std::array<size_t, 4U> neighbors{};
        size_t neighborCount{0U};
        if (current % GRID_COLUMNS != 0U)
        {
            neighbors[neighborCount++] = current - 1U;
        }
        if (current % GRID_COLUMNS < GRID_COLUMNS - 1U)
        {
            neighbors[neighborCount++] = current + 1U;
        }
        if (current / GRID_COLUMNS > yMin)
        {
            neighbors[neighborCount++] = current - GRID_COLUMNS;
        }
        if (current / GRID_COLUMNS < GRID_ROWS - 1U)
        {
            neighbors[neighborCount++] = current + GRID_COLUMNS;
        }
        for (size_t idx{0U}; idx < neighborCount; ++idx)
        {
            if (visited[neighbors[idx]] || std::find(snake.begin(), snake.end(), neighbors[idx]) != snake.end())
            {
                continue;
            }
            visited[neighbors[idx]] = true;
            from[neighbors[idx]] = current;
            frontier[frontierTail++] = neighbors[idx];
        }
    }
    return findStepAny();
}

std::optional<size_t> SnakeMode::findStepAny() const
{
    const uint8_t yMin{static_cast<uint8_t>(clock == nullptr ? 0U : 5U)};
    const size_t start{snake.back()};
    std::array<size_t, 4U> fallback{};
    size_t fallbackCount{0U};
    if (start % GRID_COLUMNS != 0U)
    {
        fallback[fallbackCount++] = start - 1U;
    }
    if (start % GRID_COLUMNS < GRID_COLUMNS - 1U)
    {
        fallback[fallbackCount++] = start + 1U;
    }
    if (start / GRID_COLUMNS > yMin)
    {
        fallback[fallbackCount++] = start - GRID_COLUMNS;
    }
    if (start / GRID_COLUMNS < GRID_ROWS - 1U)
    {
        fallback[fallbackCount++] = start + GRID_COLUMNS;
    }
    for (size_t idx{fallbackCount}; idx > 1U; --idx)
    {
        std::swap(fallback[idx - 1U], fallback[static_cast<size_t>(random(static_cast<long>(idx)))]);
    }
    for (size_t idx{0U}; idx < fallbackCount; ++idx)
    {
        if (std::find(snake.begin(), snake.end(), fallback[idx]) == snake.end())
        {
            return fallback[idx];
        }
    }
    return std::nullopt;
}

/**
 * @brief Advances the snake toward its target.
 *
 * Extends the snake when it reaches the target, otherwise updates its trail
 * and removes the tail. Starts the blinking stage when no movement is available.
 */
void SnakeMode::move()
{
    if (millis() - lastMillis > snake.size() + INT8_MAX)
    {
        const std::optional<size_t> step{findStepPath()};
        if (step.has_value())
        {
            snake.push_back(step.value());
            if (snake.back() == target)
            {
                Display.setPixel(target, UINT8_MAX);
                setTarget();
            }
            else
            {
                const uint8_t step{static_cast<uint8_t>(UINT8_MAX / snake.size())};
                for (size_t idx{0U}; idx < snake.size(); ++idx)
                {
                    Display.setPixel(snake[idx], step * (idx + 1U));
                }
                Display.setPixel(snake.front(), 0U);
                snake.pop_front();
            }
        }
        else
        {
            setDead();
        }
        lastMillis = millis();
    }
}

/**
 * @brief Blinks the snake's pixels and advances to cleanup after six toggles.
 */
void SnakeMode::blink()
{
    if (millis() - lastMillis > UINT8_MAX)
    {
        const uint8_t brightness{static_cast<uint8_t>((blinkCount & 0b1U) == 0U ? 0U : UINT8_MAX)};
        for (const size_t &pixel : snake)
        {
            Display.setPixel(pixel, brightness);
        }
        if (++blinkCount >= 6U)
        {
            stage = Stage::REMOVE;
        }
        lastMillis = millis();
    }
}

void SnakeMode::clean()
{
    if (millis() - lastMillis > INT8_MAX && !snake.empty())
    {
        Display.setPixel(snake.front(), 0U);
        snake.pop_front();
        lastMillis = millis();
    }
    else if (snake.empty())
    {
        Display.setPixel(target, 0U);
        stage = Stage::READY;
    }
}

void SnakeMode::setDead()
{
    blinkCount = 0U;
    lastMillis = millis();
    stage = Stage::DEATH;
}

/**
 * @brief Selects an unoccupied display position as the snake's target.
 *
 * The target is assigned a random brightness and is placed below the clock area
 * when the clock is enabled.
 */
void SnakeMode::setTarget()
{
    const size_t offset{static_cast<size_t>(clock == nullptr ? 0U : 5U * GRID_COLUMNS)};
    for (size_t idx{offset}; idx < GRID_COLUMNS * GRID_ROWS; ++idx)
    {
        if (Display.getPixel(idx) == 0U)
        {
            while (Display.getPixel(target) != 0U)
            {
                target =
                    static_cast<size_t>(random(static_cast<long>(offset), static_cast<long>(GRID_COLUMNS * GRID_ROWS)));
            }
            Display.setPixel(target, static_cast<uint8_t>(random(1L, static_cast<long>(0b1U << 8U))));
            return;
        }
    }
    setDead();
}

/**
 * @brief Enables or disables the Snake mode clock.
 *
 * Persists the clock setting, updates the clock handler and target as needed,
 * and transmits the updated configuration.
 *
 * @param _clock Whether the clock should be enabled.
 */
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
        Display.setPixel(target, 0U);
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
