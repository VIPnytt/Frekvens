#if MODE_CLOCK

#include "modes/ClockMode.h"

#include "extensions/HomeAssistantExtension.h"
#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"
#include "services/FontsService.h" // NOLINT(misc-include-cleaner)

#include <Preferences.h>

void ClockMode::configure()
{
    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("font"))
    {
        fontName = Storage.getString("font").c_str();
    }
    if (Storage.isKey("ticking"))
    {
        ticking = Storage.getBool("ticking");
    }
    Storage.end();
#if EXTENSION_HOMEASSISTANT
    const std::string topic{std::string("frekvens/" HOSTNAME "/").append(name)};
    const HomeAssistantExtension &_ha = Extensions.HomeAssistant();
    {
        const std::string id{std::string(name).append("_font")};
        JsonObject component{(*_ha.discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"font":"{{value}}"})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:format-font");
        component[HomeAssistantAbbreviations::name].set(std::string(name).append(" font"));
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        JsonArray options{component[HomeAssistantAbbreviations::options].to<JsonArray>()};
        for (const std::string_view _font : fontNames)
        {
            options.add(_font);
        }
        component[HomeAssistantAbbreviations::platform].set("select");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(_ha.uniquePrefix + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.font}}");
    }
    {
        const std::string id{std::string(name).append("_ticking")};
        JsonObject component{(*_ha.discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"ticking":{{value}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:progress-clock");
        component[HomeAssistantAbbreviations::name].set(std::string(name).append(" ticking"));
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::payload_off].set("false");
        component[HomeAssistantAbbreviations::payload_on].set("true");
        component[HomeAssistantAbbreviations::platform].set("switch");
        component[HomeAssistantAbbreviations::state_off].set("False");
        component[HomeAssistantAbbreviations::state_on].set("True");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(_ha.uniquePrefix + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.ticking}}");
    }
#endif // EXTENSION_HOMEASSISTANT
    transmit();
}

void ClockMode::begin() { pending = true; }

void ClockMode::handle()
{
    if (getLocalTime(&local))
    {
        if (minute != local.tm_min || hour != local.tm_hour || pending)
        {
            hour = local.tm_hour;
            minute = local.tm_min;
            drawDigits();
            pending = false;
        }
        if (ticking && second != local.tm_sec)
        {
            drawTicker();
        }
    }
}

void ClockMode::drawDigits()
{
    const std::unique_ptr<const FontModule> font = Fonts.get(fontName);
    const TextHandler hh1(std::to_string(hour / 10), *font);
    const TextHandler hh2(std::to_string(hour % 10), *font);
    const TextHandler mm1(std::to_string(minute / 10), *font);
    const TextHandler mm2(std::to_string(minute % 10), *font);
    const uint8_t fontWidth{max({hh1.getWidth(), hh2.getWidth(), mm1.getWidth(), mm2.getWidth()})};
    strikethrough = mm2.getHeight() > 6;
    Display.clearFrame();
    hh1.draw((GRID_COLUMNS / 2) - 1 - fontWidth + ((fontWidth - hh1.getWidth()) / 2),
             (GRID_ROWS / 2) - 1 - hh1.getHeight());
    hh2.draw((GRID_COLUMNS / 2) + 1 + ((fontWidth - hh2.getWidth()) / 2), (GRID_ROWS / 2) - 1 - hh2.getHeight());
    mm1.draw((GRID_COLUMNS / 2) - 1 - fontWidth + ((fontWidth - mm1.getWidth()) / 2),
             (GRID_COLUMNS / 2) + static_cast<int8_t>(strikethrough));
    mm2.draw((GRID_COLUMNS / 2) + 1 + ((fontWidth - mm2.getWidth()) / 2),
             (GRID_COLUMNS / 2) + static_cast<int8_t>(strikethrough));
}

void ClockMode::drawTicker()
{
    if (strikethrough)
    {
        Display.setPixel((GRID_COLUMNS / 2) - (60 / 4 / 2) - 1 + ((second + 2) / 4),
                         second % 2 == 0 ? (GRID_ROWS / 2) - 1 : GRID_ROWS / 2,
                         0);
    }
    second = local.tm_sec;
    if (strikethrough)
    {
        Display.setPixel((GRID_COLUMNS / 2) - (60 / 4 / 2) - 1 + ((second + 2) / 4),
                         second % 2 == 0 ? (GRID_ROWS / 2) - 1 : GRID_ROWS / 2,
                         INT8_MAX);
    }
    else if (second < 8)
    {
        Display.setPixel((GRID_COLUMNS / 2) + second - 1, (GRID_ROWS / 2) - 8, 0);
        Display.setPixel((GRID_COLUMNS / 2) + second, (GRID_ROWS / 2) - 8, INT8_MAX);
    }
    else if (second < 8 + 15)
    {
        Display.setPixel((GRID_COLUMNS / 2) + 7, (GRID_ROWS / 2) - 16 + second, 0);
        Display.setPixel((GRID_COLUMNS / 2) + 7, (GRID_ROWS / 2) - 15 + second, INT8_MAX);
    }
    else if (second < 8 + 30)
    {
        Display.setPixel((GRID_COLUMNS / 2) + 30 - second, (GRID_ROWS / 2) + 7, 0);
        Display.setPixel((GRID_COLUMNS / 2) + 29 - second, (GRID_ROWS / 2) + 7, INT8_MAX);
    }
    else if (second < 8 + 45)
    {
        Display.setPixel((GRID_COLUMNS / 2) - 8, (GRID_ROWS / 2) + 45 - second, 0);
        Display.setPixel((GRID_COLUMNS / 2) - 8, (GRID_ROWS / 2) + 44 - second, INT8_MAX);
    }
    else
    {
        Display.setPixel((GRID_COLUMNS / 2) - 61 + second, (GRID_ROWS / 2) - 8, 0);
        Display.setPixel((GRID_COLUMNS / 2) - 60 + second, (GRID_ROWS / 2) - 8, INT8_MAX);
    }
}

void ClockMode::setFont(std::string_view _fontName)
{
    if (std::unique_ptr<const FontModule> _font = Fonts.get(_fontName))
    {
        fontName = _font->name.data();
        Preferences Storage;
        Storage.begin(name);
        Storage.putString("font", fontName.c_str());
        Storage.end();
        pending = true;
        transmit();
    }
}

void ClockMode::setTicking(bool _ticking)
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

void ClockMode::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["font"].set(fontName);
    JsonArray _fonts{doc["fonts"].to<JsonArray>()};
    for (const std::string_view _font : fontNames)
    {
        _fonts.add(_font);
    }
    doc["ticking"].set(ticking);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void ClockMode::onReceive(JsonObjectConst payload,
                          std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Font
    if (payload["font"].is<std::string_view>())
    {
        setFont(payload["font"].as<std::string_view>());
    }
    // Ticking
    if (payload["ticking"].is<bool>())
    {
        setTicking(payload["ticking"].as<bool>());
    }
}

#endif // MODE_CLOCK
