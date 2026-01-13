#if MODE_LARGECLOCK

#include <Preferences.h>

#include "config/constants.h"
#include "extensions/HomeAssistantExtension.h"
#include "fonts/MediumFont.h"
#include "handlers/TextHandler.h"
#include "modes/LargeClockMode.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void LargeClockMode::configure()
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
        Storage.end();
        transmit();
    }
    else
    {
        Storage.end();
    }
}

void LargeClockMode::begin()
{
    pending = true;
}

void LargeClockMode::handle()
{
    if (getLocalTime(&local))
    {
        if (minute != local.tm_min || hour != local.tm_hour || pending)
        {
            hour = local.tm_hour;
            minute = local.tm_min;
            Display.clearFrame();
            {
                TextHandler h1 = TextHandler(std::to_string(hour / 10), FontMedium);
                h1.draw(GRID_COLUMNS / 2 - 1 - (7 - h1.getWidth()) / 2 - h1.getWidth(), GRID_ROWS / 2 - 1 - (7 - h1.getHeight()) / 2 - h1.getHeight());
            }
            {
                TextHandler h2 = TextHandler(std::to_string(hour % 10), FontMedium);
                h2.draw(GRID_COLUMNS / 2 + 1 + (7 - h2.getWidth()) / 2, GRID_ROWS / 2 - 1 + (7 - h2.getHeight()) / 2 - h2.getHeight());
            }
            {
                TextHandler m1 = TextHandler(std::to_string(minute / 10), FontMedium);
                m1.draw(GRID_COLUMNS / 2 - 1 - (7 - m1.getWidth()) / 2 - m1.getWidth(), GRID_ROWS / 2 + 1 - (7 - m1.getHeight()) / 2);
            }
            {
                TextHandler m2 = TextHandler(std::to_string(minute % 10), FontMedium);
                m2.draw(GRID_COLUMNS / 2 + 1 + (7 - m2.getWidth()) / 2, GRID_ROWS / 2 + 1 + (7 - m2.getHeight()) / 2);
            }
            pending = false;
        }
        if (second != local.tm_sec && ticking)
        {
            Display.setPixel(GRID_COLUMNS / 2 - 8 + (second + 2) / 4, second % 2 ? GRID_ROWS / 2 : GRID_ROWS / 2 - 1, 0);
            second = local.tm_sec;
            Display.setPixel(GRID_COLUMNS / 2 - 8 + (second + 2) / 4, second % 2 ? GRID_ROWS / 2 : GRID_ROWS / 2 - 1, INT8_MAX);
        }
    }
}

void LargeClockMode::setTicking(const bool _ticking)
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

void LargeClockMode::transmit()
{
    JsonDocument doc;
    doc["ticking"] = ticking;
    Device.transmit(doc, name);
}

void LargeClockMode::onReceive(const JsonDocument doc, const char *const source)
{
    // Toggle ticking
    if (doc["ticking"].is<bool>())
    {
        setTicking(doc["ticking"].as<bool>());
    }
}

#endif // MODE_LARGECLOCK
