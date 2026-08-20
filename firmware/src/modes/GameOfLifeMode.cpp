#if MODE_GAMEOFLIFE

#include "modes/GameOfLifeMode.h"

#include "config/constants.h"                  // NOLINT(misc-include-cleaner)
#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

#include <nvs.h>
#include <vector>

static_assert(GRID_COLUMNS >= 16U, __STRING(MODE_GAMEOFLIFE) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 8U, __STRING(MODE_GAMEOFLIFE) " is not compatible with this device's display size.");

void GameOfLifeMode::configure()
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
 * @brief Initializes the Game of Life mode from its persisted clock setting.
 *
 * Enables the clock display, reserves the top five rows, clears the clock, and
 * sets maximum brightness when the persisted setting is enabled.
 */
void GameOfLifeMode::begin()
{
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        uint8_t _clock{0U};
        if (nvs_get_u8(handle, "clock", &_clock) == ESP_OK && static_cast<bool>(_clock))
        {
            yMin = 5U;
            clock = std::make_unique<ClockHandler>();
            clock->clear();
            brightness = INT8_MAX;
        }
        nvs_close(handle);
    }
}

/**
 * @brief Updates the optional clock and advances the Conway's Game of Life simulation.
 */
void GameOfLifeMode::handle()
{
    if (clock != nullptr)
    {
        clock->handle();
    }
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis <= UINT8_MAX || !Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis <= UINT8_MAX)
#endif // EXTENSION_MICROPHONE
    {
        return;
    }
    std::vector<bool> seeds(GRID_COLUMNS * (GRID_ROWS - yMin), false);
    for (uint8_t idx{active}; idx < static_cast<uint8_t>(GRID_COLUMNS * (GRID_ROWS - yMin) / (0b1U << 4U)); ++idx)
    {
        seeds[random(1, GRID_COLUMNS - 1) + (random(yMin + 1U, GRID_ROWS - 1) * (GRID_COLUMNS - yMin))] = true;
    }
    lastMillis = millis();
    active = 0U;
    for (uint8_t x{0U}; x < GRID_COLUMNS; ++x)
    {
        for (uint8_t y{yMin}; y < GRID_ROWS; ++y)
        {
            uint8_t count{0U};
            for (uint8_t _x{static_cast<uint8_t>(max<int16_t>(x - 1, 0))}; _x <= x + 1U && _x < GRID_COLUMNS; ++_x)
            {
                for (uint8_t _y{static_cast<uint8_t>(max<int16_t>(yMin, y - 1U))}; _y <= y + 1U && _y < GRID_ROWS; ++_y)
                {
                    if ((_x != x || _y != y) &&
                        (seeds[_x + (_y * (GRID_COLUMNS - yMin))] || Display.getPixel(_x, _y) != 0U))
                    {
                        ++count;
                    }
                }
            }
            const bool lit{seeds[x + (y * (GRID_COLUMNS - yMin))] || Display.getPixel(x, y) != 0U};
            if (lit && (count < 2U || count > 3U))
            {
                Display.setPixel(x, y, 0U);
            }
            else if (!lit && count == 3U)
            {
                Display.setPixel(x, y, brightness);
                ++active;
            }
        }
    }
}

/**
 * @brief Enables or disables the clock display and updates the display configuration.
 *
 * @param _clock Whether to enable the clock display.
 */
void GameOfLifeMode::setClock(bool _clock)
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
        yMin = 5U;
        clock = std::make_unique<ClockHandler>();
        clock->clear();
        brightness = INT8_MAX;
    }
    else
    {
        clock.reset();
        yMin = 0U;
        brightness = UINT8_MAX;
    }
    transmit();
}

/**
 * @brief Transmits the current clock-enabled state.
 */
void GameOfLifeMode::transmit()
{
    JsonDocument doc{};
    doc["clock"].set(clock != nullptr);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

/**
 * @brief Applies a received clock configuration.
 *
 * @param payload Incoming JSON payload containing an optional Boolean `clock` value.
 * @param source Source identifier for the received payload.
 */
void GameOfLifeMode::onReceive(JsonObjectConst payload, std::string_view source)
{
    // Clock
    if (payload["clock"].is<bool>())
    {
        setClock(payload["clock"].as<bool>());
    }
}

#if EXTENSION_HOMEASSISTANT
/**
 * @brief Adds Home Assistant discovery metadata for the Game of Life clock switch.
 *
 * @param discovery JSON document to which the switch configuration is added.
 * @param topic Base topic for the device mode.
 * @param unique Unique identifier prefix for the switch.
 */
void GameOfLifeMode::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_clock")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"clock":{{value}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:one-up");
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

#endif // MODE_GAMEOFLIFE
