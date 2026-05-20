#if MODE_GAMEOFLIFE

#include "modes/GameOfLifeMode.h"

#include "config/constants.h"     // NOLINT(misc-include-cleaner)
#include "fonts/MiniFont.h"       // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

#include <nvs.h>
#include <vector>

static_assert(GRID_COLUMNS >= 16U, __STRING(MODE_GAMEOFLIFE) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 8U, __STRING(MODE_GAMEOFLIFE) " is not compatible with this device's display size.");

void GameOfLifeMode::configure()
{
    bool _pending{false};
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        uint8_t _clock{0U};
        _pending = nvs_get_u8(handle, "clock", &_clock) == ESP_OK;
        nvs_close(handle);
        if (_pending)
        {
            setClock(static_cast<bool>(_clock));
        }
    }
    if (!_pending)
    {
        transmit();
    }
}

void GameOfLifeMode::begin() { pending = true; }

void GameOfLifeMode::handle()
{
    if (millis() - lastMillis <= UINT8_MAX)
    {
        return;
    }
    if (clock && getLocalTime(&local) && (minute != local.tm_min || hour != local.tm_hour || pending))
    {
        hour = local.tm_hour;
        minute = local.tm_min;
#ifdef CLOCK_12H
        const int hour{(local.tm_hour + 11) % 12 + 1};
#endif // CLOCK_12H
        Display.drawRectangle((GRID_COLUMNS / 2U) - 8U, 0U, (GRID_COLUMNS / 2U) + 7U, 4U, true, 0U);
        const MiniFont font;
        TextHandler(std::to_string(hour / 10), font).draw(GRID_COLUMNS / 2U - 8U, 0U);
        TextHandler(std::to_string(hour % 10), font).draw(GRID_COLUMNS / 2U - 4U, 0U);
        TextHandler(std::to_string(minute / 10), font).draw(GRID_COLUMNS / 2U + 1U, 0U);
        TextHandler(std::to_string(minute % 10), font).draw(GRID_COLUMNS / 2U + 5U, 0U);
        pending = false;
    }
    std::vector<bool> seeds(GRID_COLUMNS * (GRID_ROWS - yMin), false);
    for (uint8_t i{active}; i < static_cast<uint8_t>(GRID_COLUMNS * (GRID_ROWS - yMin) / (1U << 4U)); ++i)
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
            // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
            for (uint8_t _x{static_cast<uint8_t>(max<int16_t>(x - 1, 0))}; _x <= x + 1U && _x < GRID_COLUMNS; ++_x)
            {
                // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
                for (uint8_t _y{static_cast<uint8_t>(max<int16_t>(yMin, y - 1U))}; _y <= y + 1U && _y < GRID_ROWS; ++_y)
                {
                    if ((_x != x || _y != y) &&
                        (seeds[_x + (_y * (GRID_COLUMNS - yMin))] || Display.getPixel(_x, _y) != 0U))
                    {
                        ++count;
                    }
                }
            }
            // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
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

void GameOfLifeMode::setClock(bool _clock)
{
    clock = _clock;
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "clock", static_cast<uint8_t>(clock)); // NOLINT(readability-implicit-bool-conversion)
        nvs_commit(handle);
        nvs_close(handle);
    }
    brightness = clock ? INT8_MAX : UINT8_MAX;
    yMin = clock ? 5U : 0U;
    pending = true;
    transmit();
}

void GameOfLifeMode::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["clock"].set(clock);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void GameOfLifeMode::onReceive(JsonObjectConst payload,
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
