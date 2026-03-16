#if MODE_CLOCK

#include "modes/ClockMode.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "extensions/HomeAssistantExtension.h"
#include "fonts/MediumBoldFont.h"
#include "fonts/MediumFont.h"     // NOLINT(misc-include-cleaner)
#include "fonts/MediumWideFont.h" // NOLINT(misc-include-cleaner)
#include "fonts/MiniFont.h"       // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

#include <Preferences.h>

void ClockMode::borderPixel(uint8_t sec, uint8_t brightness) // NOLINT(bugprone-easily-swappable-parameters)
{
    // Segment boundaries (number of steps in each segment)
    static constexpr uint8_t SEG1 = GRID_COLUMNS / 2U;        // top-right half
    static constexpr uint8_t SEG2 = SEG1 + GRID_ROWS - 1U;    // right edge (y 1..GRID_ROWS-1)
    static constexpr uint8_t SEG3 = SEG2 + GRID_COLUMNS - 1U; // bottom edge (x GRID_COLUMNS-2..0)
    static constexpr uint8_t SEG4 = SEG3 + GRID_ROWS - 1U;    // left edge (y GRID_ROWS-2..0)
    // remaining [SEG4,60): top-left half (x 1..GRID_COLUMNS/2-1)

    uint8_t x = 0;
    uint8_t y = 0;

    if (sec < SEG1)
    {
        x = (GRID_COLUMNS / 2U) + sec;
        y = 0U;
    }
    else if (sec < SEG2)
    {
        x = GRID_COLUMNS - 1U;
        y = sec - (GRID_COLUMNS / 2U) + 1U;
    }
    else if (sec < SEG3)
    {
        x = SEG2 + GRID_COLUMNS - 2U - sec;
        y = GRID_ROWS - 1U;
    }
    else if (sec < SEG4)
    {
        x = 0U;
        y = SEG3 + GRID_ROWS - 2U - sec;
    }
    else
    {
        x = sec - SEG4 + 1U;
        y = 0U;
    }

    Display.setPixel(x, y, brightness);
}

void ClockMode::configure()
{
    fonts = {
        FontMini,
        FontMedium,
        FontMediumBold,
        FontMediumWide,
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
        cellSize = TextHandler("0", font).getHeight();
    }

#if EXTENSION_HOMEASSISTANT
    const std::string topic{std::string("frekvens/" HOSTNAME "/").append(name)};
    {
        const std::string id{std::string(name).append("_font")};
        JsonObject component{(*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"font":"{{value}}"})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:format-font");
        component[HomeAssistantAbbreviations::name].set(std::string(name).append(" font"));
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        JsonArray options{component[HomeAssistantAbbreviations::options].to<JsonArray>()};
        for (const FontModule *_font : fonts)
        {
            options.add(_font->name);
        }
        component[HomeAssistantAbbreviations::platform].set("select");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(HomeAssistant->uniquePrefix + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.font}}");
    }
    {
        const std::string id{std::string(name).append("_ticking")};
        JsonObject component{(*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
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
        component[HomeAssistantAbbreviations::unique_id].set(HomeAssistant->uniquePrefix + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.ticking}}");
    }
#endif // EXTENSION_HOMEASSISTANT

    transmit();
}

void ClockMode::begin() { pending = true; }

void ClockMode::drawDigits()
{
    Display.clearFrame();

    TextHandler hh1(std::to_string(hour / 10), font);
    TextHandler hh2(std::to_string(hour % 10), font);
    TextHandler mm1(std::to_string(minute / 10), font);
    TextHandler mm2(std::to_string(minute % 10), font);

    // Small font: digits flush to the center gap (no cell padding).
    // Large font: digits centred in a cellSize-wide cell on each side of the gap.
    const int8_t yTop = (GRID_ROWS / 2) - 1 - cellSize;
    const int8_t yBot = (cellSize <= 5) ? (GRID_ROWS / 2) : (GRID_ROWS / 2) + 1;

    auto xPad = [this](uint8_t w) -> int8_t { return (cellSize <= 5) ? 0 : (cellSize - w) / 2; };

    hh1.draw((GRID_COLUMNS / 2) - 1 - xPad(hh1.getWidth()) - hh1.getWidth(), yTop);
    hh2.draw((GRID_COLUMNS / 2) + 1 + xPad(hh2.getWidth()), yTop);
    mm1.draw((GRID_COLUMNS / 2) - 1 - xPad(mm1.getWidth()) - mm1.getWidth(), yBot);
    mm2.draw((GRID_COLUMNS / 2) + 1 + xPad(mm2.getWidth()), yBot);
}

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
            if (cellSize <= 5)
            {
                // Small font: sweep a pixel clockwise around the grid border
                borderPixel(second, 0);
                second = static_cast<uint8_t>(local.tm_sec);
                borderPixel(second, INT8_MAX);
            }
            else
            {
                // Large font: move a pixel along the center gap between digit rows
                Display.setPixel((GRID_COLUMNS / 2) - 8 + ((second + 2) / 4),
                                 (second % 2) == 0 ? (GRID_ROWS / 2) - 1 : GRID_ROWS / 2,
                                 0);
                second = static_cast<uint8_t>(local.tm_sec);
                Display.setPixel((GRID_COLUMNS / 2) - 8 + ((second + 2) / 4),
                                 (second % 2) == 0 ? (GRID_ROWS / 2) - 1 : GRID_ROWS / 2,
                                 INT8_MAX);
            }
        }
    }
}

void ClockMode::setFont(const char *fontName)
{
    if (font == nullptr || strcmp(font->name, fontName) != 0)
    {
        for (FontModule *_font : fonts)
        {
            if (!strcmp(_font->name, fontName))
            {
                font = _font;
                cellSize = TextHandler("0", font).getHeight();
                Preferences Storage;
                Storage.begin(name);
                Storage.putString("font", font->name);
                Storage.end();
                pending = true;
                transmit();
                return;
            }
        }
        ESP_LOGD(name, "unsupported font %s", fontName); // NOLINT(cppcoreguidelines-avoid-do-while)
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
    doc["font"].set(font->name);
    JsonArray _fonts{doc["fonts"].to<JsonArray>()};
    for (const FontModule *_font : fonts)
    {
        _fonts.add(_font->name);
    }
    doc["ticking"].set(ticking);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void ClockMode::onReceive(JsonObjectConst payload,
                          const char *source) // NOLINT(misc-unused-parameters)
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

#endif // MODE_CLOCK
