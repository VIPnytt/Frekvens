#if MODE_GAMEOFLIFE

#include "modes/GameOfLifeMode.h"

#include "config/constants.h"
#include "extensions/HomeAssistantExtension.h"
#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

#include <Preferences.h>

void GameOfLifeMode::configure()
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
        component[HomeAssistantAbbreviations::icon] = "mdi:one-up";
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
            TextHandler(std::to_string(hour / 10), FontMini).draw(GRID_COLUMNS / 2 - 8, 0);
            TextHandler(std::to_string(hour % 10), FontMini).draw(GRID_COLUMNS / 2 - 4, 0);
            TextHandler(std::to_string(minute / 10), FontMini).draw(GRID_COLUMNS / 2 + 1, 0);
            TextHandler(std::to_string(minute % 10), FontMini).draw(GRID_COLUMNS / 2 + 5, 0);
            pending = false;
        }
        bool seeds[GRID_COLUMNS * (GRID_ROWS - (clock ? 5 : 0))] = {false};
        for (uint8_t i = active; i < GRID_COLUMNS * (GRID_ROWS - (clock ? 5 : 0)) / (1 << 4); ++i)
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
                uint8_t n = 0;
                for (uint8_t _x = static_cast<uint8_t>(std::max<int16_t>(x - 1, 0)); _x <= x + 1 && _x < GRID_COLUMNS;
                     ++_x)
                {
                    for (uint8_t _y = static_cast<uint8_t>(std::max<int16_t>(clock ? 5 : 0, y - 1));
                         _y <= y + 1 && _y < GRID_ROWS;
                         ++_y)
                    {
                        if ((_x != x || _y != y) &&
                            (seeds[_x + (_y * (GRID_COLUMNS - (clock ? 5 : 0)))] || Display.getPixel(_x, _y) != 0))
                        {
                            ++n;
                        }
                    }
                }
                const bool lit = seeds[x + (y * (GRID_COLUMNS - (clock ? 5 : 0)))] || Display.getPixel(x, y) != 0;
                if (lit && (n < 2 || n > 3))
                {
                    Display.setPixel(x, y, 0);
                }
                else if (!lit && n == 3)
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
    if (_clock != clock)
    {
        clock = _clock;
        Preferences Storage;
        Storage.begin(name);
        Storage.putBool("clock", clock);
        Storage.end();
        pending = true;
        transmit();
    }
}

void GameOfLifeMode::transmit()
{
    JsonDocument doc;
    doc["clock"] = clock;
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void GameOfLifeMode::onReceive(JsonObjectConst payload, const char *source)
{
    // Clock
    if (payload["clock"].is<bool>())
    {
        setClock(payload["clock"].as<bool>());
    }
}

#endif // MODE_GAMEOFLIFE
