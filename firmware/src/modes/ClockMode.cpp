#if MODE_CLOCK

#include "modes/ClockMode.h"

#include "config/constants.h"
#include "extensions/HomeAssistantExtension.h"
#include "fonts/MediumBoldFont.h"
#include "fonts/MediumFont.h"
#include "fonts/MediumRegularFont.h"
#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

#include <Preferences.h>

void ClockMode::borderPixel(uint8_t sec, uint8_t brightness)
{
    // Segment boundaries (number of steps in each segment)
    static constexpr uint8_t SEG1 = GRID_COLUMNS / 2u;        // top-right half
    static constexpr uint8_t SEG2 = SEG1 + GRID_ROWS - 1u;    // right edge (y 1..GRID_ROWS-1)
    static constexpr uint8_t SEG3 = SEG2 + GRID_COLUMNS - 1u; // bottom edge (x GRID_COLUMNS-2..0)
    static constexpr uint8_t SEG4 = SEG3 + GRID_ROWS - 1u;    // left edge (y GRID_ROWS-2..0)
    // remaining [SEG4,60): top-left half (x 1..GRID_COLUMNS/2-1)

    uint8_t x;
    uint8_t y;

    if (sec < SEG1)
    {
        x = (GRID_COLUMNS / 2u) + sec;
        y = 0u;
    }
    else if (sec < SEG2)
    {
        x = GRID_COLUMNS - 1u;
        y = sec - (GRID_COLUMNS / 2u) + 1u;
    }
    else if (sec < SEG3)
    {
        x = SEG2 + GRID_COLUMNS - 2u - sec;
        y = GRID_ROWS - 1u;
    }
    else if (sec < SEG4)
    {
        x = 0u;
        y = SEG3 + GRID_ROWS - 2u - sec;
    }
    else
    {
        x = sec - SEG4 + 1u;
        y = 0u;
    }

    Display.setPixel(x, y, brightness);
}

void ClockMode::configure()
{
    fonts = {
        FontMini,
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
        cellSize = TextHandler("0", font).getHeight();
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

void ClockMode::begin() { pending = true; }

void ClockMode::drawDigits()
{
    Display.clearFrame();

    TextHandler h1(std::to_string(hour / 10), font);
    TextHandler h2(std::to_string(hour % 10), font);
    TextHandler m1(std::to_string(minute / 10), font);
    TextHandler m2(std::to_string(minute % 10), font);

    // Small font: digits flush to the center gap (no cell padding).
    // Large font: digits centred in a cellSize-wide cell on each side of the gap.
    const int8_t yTop = (GRID_ROWS / 2) - 1 - cellSize;
    const int8_t yBot = (cellSize <= 5) ? (GRID_ROWS / 2) : (GRID_ROWS / 2) + 1;

    auto xPad = [this](uint8_t w) -> int8_t { return (cellSize <= 5) ? 0 : (cellSize - w) / 2; };

    h1.draw((GRID_COLUMNS / 2) - 1 - xPad(h1.getWidth()) - h1.getWidth(), yTop);
    h2.draw((GRID_COLUMNS / 2) + 1 + xPad(h2.getWidth()), yTop);
    m1.draw((GRID_COLUMNS / 2) - 1 - xPad(m1.getWidth()) - m1.getWidth(), yBot);
    m2.draw((GRID_COLUMNS / 2) + 1 + xPad(m2.getWidth()), yBot);
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
        ESP_LOGD(name, "unsupported font %s", fontName);
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

void ClockMode::onReceive(JsonObjectConst payload, const char *source)
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
