#if MODE_CLOCK

#include "modes/ClockMode.h"

#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)
#include "services/FontsService.h"      // NOLINT(misc-include-cleaner)

#include <nvs.h>

static_assert(GRID_COLUMNS >= 16U, __STRING(MODE_CLOCK) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 16U, __STRING(MODE_CLOCK) " is not compatible with this device's display size.");

void ClockMode::configure()
{
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        size_t length{0U};
        if (nvs_get_str(handle, "font", nullptr, &length) == ESP_OK && length > 0U)
        {
            fontName.resize(length - 1U);
            nvs_get_str(handle, "font", fontName.data(), &length);
        }
        uint8_t _ticking{0U};
        if (nvs_get_u8(handle, "ticking", &_ticking) == ESP_OK)
        {
            ticking = static_cast<bool>(_ticking);
        }
        nvs_close(handle);
    }
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
            const std::unique_ptr<const FontModule> font{Fonts.get(fontName)};
            const TextHandler hh1(std::to_string(hour / 10), *font);
            const TextHandler hh2(std::to_string(hour % 10), *font);
            const TextHandler mm1(std::to_string(minute / 10), *font);
            const TextHandler mm2(std::to_string(minute % 10), *font);
            const uint8_t fontWidth{max({hh1.getWidth(), hh2.getWidth(), mm1.getWidth(), mm2.getWidth()})};
            strikethrough = mm2.getHeight() > 6U;
            Display.clearFrame();
            hh1.draw(static_cast<int16_t>((GRID_COLUMNS / 2U) - 1U - fontWidth + ((fontWidth - hh1.getWidth()) / 2U)),
                     static_cast<int8_t>((GRID_ROWS / 2U) - 1U - hh1.getHeight()));
            hh2.draw(static_cast<int16_t>((GRID_COLUMNS / 2U) + 1U + ((fontWidth - hh2.getWidth()) / 2U)),
                     static_cast<int8_t>((GRID_ROWS / 2U) - 1U - hh2.getHeight()));
            mm1.draw(static_cast<int16_t>((GRID_COLUMNS / 2U) - 1U - fontWidth + ((fontWidth - mm1.getWidth()) / 2U)),
                     static_cast<int8_t>((GRID_COLUMNS / 2U) + (strikethrough ? 1U : 0U)));
            mm2.draw(static_cast<int16_t>((GRID_COLUMNS / 2U) + 1U + ((fontWidth - mm2.getWidth()) / 2U)),
                     static_cast<int8_t>((GRID_COLUMNS / 2U) + (strikethrough ? 1U : 0U)));
            pending = false;
        }
        if (ticking && second != local.tm_sec)
        {
            drawTicker(0U);
            second = local.tm_sec;
            drawTicker(INT8_MAX);
        }
    }
}

void ClockMode::drawTicker(uint8_t brightness)
{
    if (strikethrough)
    {
        Display.setPixel((GRID_COLUMNS / 2U) - (60U / 4U / 2U) - 1U + ((second + 2U) / 4U),
                         (static_cast<unsigned>(second) & 1U) == 0U ? (GRID_ROWS / 2U) - 1U : GRID_ROWS / 2U,
                         brightness);
    }
    else if (second < 8U)
    {
        Display.setPixel((GRID_COLUMNS / 2U) + second, (GRID_ROWS / 2U) - 8U, brightness);
    }
    else if (second < 8U + 15U)
    {
        Display.setPixel((GRID_COLUMNS / 2U) + 7U, (GRID_ROWS / 2U) - 15U + second, brightness);
    }
    else if (second < 8U + 30U)
    {
        Display.setPixel((GRID_COLUMNS / 2U) + 29U - second, (GRID_ROWS / 2U) + 7U, brightness);
    }
    else if (second < 8U + 45U)
    {
        Display.setPixel((GRID_COLUMNS / 2U) - 8U, (GRID_ROWS / 2U) + 44U - second, brightness);
    }
    else
    {
        Display.setPixel((GRID_COLUMNS / 2U) - 60U + second, (GRID_ROWS / 2U) - 8U, brightness);
    }
}

void ClockMode::setFont(std::string_view _fontName)
{
    if (std::unique_ptr<const FontModule> _font{Fonts.get(_fontName)})
    {
        fontName = _font->name;
        nvs_handle_t handle{};
        if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
        {
            nvs_set_str(handle, "font", fontName.c_str());
            nvs_commit(handle);
            nvs_close(handle);
        }
        pending = true;
        transmit();
    }
}

void ClockMode::setTicking(bool _ticking)
{
    ticking = _ticking;
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "ticking", static_cast<uint8_t>(ticking)); // NOLINT(readability-implicit-bool-conversion)
        nvs_commit(handle);
        nvs_close(handle);
    }
    pending = true;
    transmit();
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

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void ClockMode::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_font")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
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
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.font}}");
    }
    {
        const std::string id{std::string(name).append("_ticking")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"ticking":{{value}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:progress-clock");
        component[HomeAssistantAbbreviations::name].set(std::string(name).append(" second indicator"));
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::payload_off].set("false");
        component[HomeAssistantAbbreviations::payload_on].set("true");
        component[HomeAssistantAbbreviations::platform].set("switch");
        component[HomeAssistantAbbreviations::state_off].set("False");
        component[HomeAssistantAbbreviations::state_on].set("True");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.ticking}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // MODE_CLOCK
