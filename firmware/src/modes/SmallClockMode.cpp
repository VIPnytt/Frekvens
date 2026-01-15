#if MODE_SMALLCLOCK

#include <Preferences.h>

#include "config/constants.h"
#include "extensions/HomeAssistantExtension.h"
#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "modes/SmallClockMode.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void SmallClockMode::configure()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_ticking");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"ticking\":{{value}}}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::enabled_by_default] = false;
        component[HomeAssistantAbbreviations::entity_category] = "config";
        component[HomeAssistantAbbreviations::icon] = "mdi:progress-clock";
        component[HomeAssistantAbbreviations::name] = std::string(name).append(" ticking");
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::payload_off] = "false";
        component[HomeAssistantAbbreviations::payload_on] = "true";
        component[HomeAssistantAbbreviations::platform] = "switch";
        component[HomeAssistantAbbreviations::state_off] = "False";
        component[HomeAssistantAbbreviations::state_on] = "True";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.ticking}}";
    }
#endif // EXTENSION_HOMEASSISTANT
    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("ticking"))
    {
        ticking = Storage.getBool("ticking");
    }
    Storage.end();
    transmit();
}

void SmallClockMode::begin()
{
    pending = true;
}

void SmallClockMode::handle()
{
    if (getLocalTime(&local))
    {
        if (minute != local.tm_min || hour != local.tm_hour || pending)
        {
            hour = local.tm_hour;
            minute = local.tm_min;
            Display.clearFrame();
            TextHandler(std::to_string(hour / 10), FontMini).draw(GRID_COLUMNS / 2 - 4, GRID_ROWS / 2 - 6);
            TextHandler(std::to_string(hour % 10), FontMini).draw(GRID_COLUMNS / 2 + 1, GRID_ROWS / 2 - 6);
            TextHandler(std::to_string(minute / 10), FontMini).draw(GRID_COLUMNS / 2 - 4, GRID_ROWS / 2);
            TextHandler(std::to_string(minute % 10), FontMini).draw(GRID_COLUMNS / 2 + 1, GRID_ROWS / 2);
            pending = false;
        }
        if (second != local.tm_sec && ticking)
        {
            second = local.tm_sec;
            if (second < 8)
            {
                Display.setPixel(GRID_COLUMNS / 2 - 1 + second, GRID_ROWS / 2 - 8, 0);
                Display.setPixel(GRID_COLUMNS / 2 + second, GRID_ROWS / 2 - 8, INT8_MAX);
            }
            else if (second < 8 + 15)
            {
                Display.setPixel(GRID_COLUMNS / 2 + 7, GRID_ROWS / 2 - 16 + second, 0);
                Display.setPixel(GRID_COLUMNS / 2 + 7, GRID_ROWS / 2 - 15 + second, INT8_MAX);
            }
            else if (second < 8 + 15 * 2)
            {
                Display.setPixel(GRID_COLUMNS / 2 + 30 - second, GRID_ROWS / 2 + 7, 0);
                Display.setPixel(GRID_COLUMNS / 2 + 29 - second, GRID_ROWS / 2 + 7, INT8_MAX);
            }
            else if (second < 8 + 15 * 3)
            {
                Display.setPixel(GRID_COLUMNS / 2 - 8, GRID_ROWS / 2 + 45 - second, 0);
                Display.setPixel(GRID_COLUMNS / 2 - 8, GRID_ROWS / 2 + 44 - second, INT8_MAX);
            }
            else
            {
                Display.setPixel(GRID_COLUMNS / 2 - 61 + second, GRID_ROWS / 2 - 8, 0);
                Display.setPixel(GRID_COLUMNS / 2 - 60 + second, GRID_ROWS / 2 - 8, INT8_MAX);
            }
        }
    }
}

void SmallClockMode::setTicking(const bool _ticking)
{
    if (_ticking != ticking)
    {
        ticking = _ticking;
        Preferences Storage;
        Storage.begin(name);
        Storage.putBool("ticking", ticking);
        Storage.end();
        pending = true;
        transmit();
    }
}

void SmallClockMode::transmit()
{
    JsonDocument doc;
    doc["ticking"] = ticking;
    Device.transmit(doc, name);
}

void SmallClockMode::onReceive(const JsonDocument doc, const char *const source)
{
    // Toggle ticking
    if (doc["ticking"].is<bool>())
    {
        setTicking(doc["ticking"].as<bool>());
    }
}

#endif // MODE_SMALLCLOCK
