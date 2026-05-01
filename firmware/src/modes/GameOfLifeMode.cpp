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

void GameOfLifeMode::configure()
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

void GameOfLifeMode::begin() { pending = true; }

void GameOfLifeMode::handle()
{
    if (millis() - lastMillis > UINT8_MAX)
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
        std::vector<bool> seeds(GRID_COLUMNS * (GRID_ROWS - (clock ? 5 : 0)), false);
        for (uint8_t i = active; i < GRID_COLUMNS * (GRID_ROWS - (clock ? 5 : 0)) / (1U << 4U); ++i)
        {
            seeds[random(1, GRID_COLUMNS - 1) +
                  (random(clock ? 6 : 1, GRID_ROWS - 1) * (GRID_COLUMNS - (clock ? 5 : 0)))] = true;
        }
        lastMillis = millis();
        active = 0;
        for (uint8_t x = 0; x < GRID_COLUMNS; ++x)
        {
            for (uint8_t y = (clock ? 5 : 0); y < GRID_ROWS; ++y)
            {
                uint8_t count = 0;
                // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
                for (uint8_t _x{static_cast<uint8_t>(std::max<int16_t>(x - 1, 0))}; _x <= x + 1 && _x < GRID_COLUMNS;
                     ++_x)
                {
                    // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
                    for (uint8_t _y{static_cast<uint8_t>(std::max<int16_t>(clock ? 5 : 0, y - 1))};
                         _y <= y + 1 && _y < GRID_ROWS;
                         ++_y)
                    {
                        if ((_x != x || _y != y) &&
                            (seeds[_x + (_y * (GRID_COLUMNS - (clock ? 5 : 0)))] || Display.getPixel(_x, _y) != 0))
                        {
                            ++count;
                        }
                    }
                }
                // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
                const bool lit{seeds[x + (y * (GRID_COLUMNS - (clock ? 5 : 0)))] || Display.getPixel(x, y) != 0};
                if (lit && (count < 2 || count > 3))
                {
                    Display.setPixel(x, y, 0);
                }
                else if (!lit && count == 3)
                {
                    Display.setPixel(x, y, clock ? INT8_MAX : UINT8_MAX);
                    ++active;
                }
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

#endif // MODE_GAMEOFLIFE
