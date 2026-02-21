#if MODE_LARGECLOCK

#include "modes/LargeClockMode.h"

#include "config/constants.h"
#include "extensions/HomeAssistantExtension.h"
#include "fonts/MediumBoldFont.h"
#include "fonts/MediumFont.h"
#include "fonts/MediumRegularFont.h"
#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

#include <Preferences.h>

void LargeClockMode::configure()
{
    fonts = {
        FontMedium,
        FontMediumRegular,
        FontMediumBold,
    };
    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("ticking"))
    {
        ticking = Storage.getBool("ticking");
    }
    if (Storage.isKey("font"))
    {
        const String _font = Storage.getString("font");
        Storage.end();
        setFont(_font.c_str());
    }
    else
    {
        Storage.end();
    }
    if (font == nullptr)
    {
        font = FontMediumBold;
    }
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_font");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = R"({"font":"{{value}}"})";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::enabled_by_default] = false;
        component[HomeAssistantAbbreviations::entity_category] = "config";
        component[HomeAssistantAbbreviations::icon] = "mdi:format-font";
        component[HomeAssistantAbbreviations::name] = std::string(name).append(" font");
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        JsonArray options = component[HomeAssistantAbbreviations::options].to<JsonArray>();
        for (const FontModule *_font : fonts)
        {
            options.add(_font->name);
        }
        component[HomeAssistantAbbreviations::platform] = "select";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.font}}";
    }
    {
        const std::string id = std::string(name).append("_ticking");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = R"({"ticking":{{value}}})";
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
    transmit();
}

void LargeClockMode::begin() { pending = true; }

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
                TextHandler h1 = TextHandler(std::to_string(hour / 10), font);
                h1.draw((GRID_COLUMNS / 2) - 1 - ((7 - h1.getWidth()) / 2) - h1.getWidth(),
                        (GRID_ROWS / 2) - 1 - ((7 - h1.getHeight()) / 2) - h1.getHeight());
            }
            {
                TextHandler h2 = TextHandler(std::to_string(hour % 10), font);
                h2.draw((GRID_COLUMNS / 2) + 1 + ((7 - h2.getWidth()) / 2),
                        (GRID_ROWS / 2) - 1 + ((7 - h2.getHeight()) / 2) - h2.getHeight());
            }
            {
                TextHandler m1 = TextHandler(std::to_string(minute / 10), font);
                m1.draw((GRID_COLUMNS / 2) - 1 - ((7 - m1.getWidth()) / 2) - m1.getWidth(),
                        (GRID_ROWS / 2) + 1 - ((7 - m1.getHeight()) / 2));
            }
            {
                TextHandler m2 = TextHandler(std::to_string(minute % 10), font);
                m2.draw((GRID_COLUMNS / 2) + 1 + ((7 - m2.getWidth()) / 2),
                        (GRID_ROWS / 2) + 1 + ((7 - m2.getHeight()) / 2));
            }
            pending = false;
        }
        if (ticking && second != local.tm_sec)
        {
            Display.setPixel((GRID_COLUMNS / 2) - 8 + ((second + 2) / 4),
                             (second % 2) == 0 ? (GRID_ROWS / 2) - 1 : GRID_ROWS / 2,
                             0);
            second = local.tm_sec;
            Display.setPixel((GRID_COLUMNS / 2) - 8 + ((second + 2) / 4),
                             (second % 2) == 0 ? (GRID_ROWS / 2) - 1 : GRID_ROWS / 2,
                             INT8_MAX);
        }
    }
}

void LargeClockMode::setFont(const char *fontName)
{
    if (font == nullptr || strcmp(font->name, fontName) != 0)
    {
        for (FontModule *_font : fonts)
        {
            if (!strcmp(_font->name, fontName))
            {
                font = _font;
                Preferences Storage;
                Storage.begin(name);
                Storage.putString("font", font->name);
                Storage.end();
                pending = true;
                transmit();
                return;
            }
        }
        ESP_LOGD(name, "unsupported font %s", fontName);
    }
}

void LargeClockMode::setTicking(bool _ticking)
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
    doc["font"] = font->name;
    JsonArray _fonts = doc["fonts"].to<JsonArray>();
    for (const FontModule *_font : fonts)
    {
        _fonts.add(_font->name);
    }
    doc["ticking"] = ticking;
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void LargeClockMode::onReceive(JsonObjectConst payload, const char *source)
{
    // Font
    if (payload["font"].is<const char *>())
    {
        setFont(payload["font"].as<const char *>());
    }
    // Ticking
    if (payload["ticking"].is<bool>())
    {
        setTicking(payload["ticking"].as<bool>());
    }
}

#endif // MODE_LARGECLOCK
