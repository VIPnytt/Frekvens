#if MODE_COUNTDOWN

#include "modes/CountdownMode.h"

#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)
#include "services/FontsService.h"

#include <array>
#include <iomanip>
#include <nvs.h>
#include <sstream>
#include <string_view>

static_assert(GRID_COLUMNS >= 8U, __STRING(MODE_COUNTDOWN) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 11U, __STRING(MODE_COUNTDOWN) " is not compatible with this device's display size.");

void CountdownMode::configure()
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
        int64_t _epoch{};
        if (nvs_get_i64(handle, "epoch", &_epoch) == ESP_OK) // NOLINT(bugprone-branch-clone)
        {
            nvs_close(handle);
            epoch = std::chrono::system_clock::time_point{std::chrono::seconds{_epoch}};
        }
        else
        {
            nvs_close(handle);
        }
    }
    transmit();
}

void CountdownMode::begin()
{
    blink = 0U;
    lower = 0U;
    upper = 0U;
}

void CountdownMode::handle()
{
    const std::chrono::nanoseconds _nanoseconds{epoch - std::chrono::system_clock::now()};
    const std::chrono::hours _hours{std::chrono::duration_cast<std::chrono::hours>(_nanoseconds)};
    const std::chrono::minutes _minutes{std::chrono::duration_cast<std::chrono::minutes>(_nanoseconds - _hours)};
    const int64_t hours{_hours.count()};
    const int64_t minutes{_minutes.count()};
    // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
    const int64_t seconds{std::chrono::duration_cast<std::chrono::seconds>(_nanoseconds - _hours - _minutes).count()};
    // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
    const uint8_t _upper{static_cast<uint8_t>(std::clamp<int64_t>(hours > 0 ? hours % 100 : minutes, 0, 99))};
    // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
    const uint8_t _lower{static_cast<uint8_t>(std::clamp<int64_t>(hours > 0 ? minutes : seconds, 0, 99))};
    if (_lower != lower || _upper != upper)
    {
        upper = _upper;
        lower = _lower;
        if (seconds >= 0 && minutes >= 0 && hours >= 0)
        {
            const std::unique_ptr<const FontModule> font{Fonts.get(fontName)};
            const TextHandler _tl(std::to_string(_upper / 10U), *font);
            const TextHandler _tr(std::to_string(_upper % 10U), *font);
            const TextHandler _bl(std::to_string(lower / 10U), *font);
            const TextHandler _br(std::to_string(lower % 10U), *font);
            const uint8_t fontWidth{max({_tl.getWidth(), _tr.getWidth(), _bl.getWidth(), _br.getWidth()})};
            Display.clearFrame();
            _tl.draw((GRID_COLUMNS / 2U) - 1U - fontWidth + ((fontWidth - _tl.getWidth()) / 2U),
                     static_cast<int8_t>((GRID_ROWS / 2U) - 1U - _tl.getHeight()));
            _tr.draw((GRID_COLUMNS / 2U) + 1U + ((fontWidth - _tr.getWidth()) / 2U),
                     static_cast<int8_t>((GRID_ROWS / 2U) - 1U - _tr.getHeight()));
            _bl.draw((GRID_COLUMNS / 2U) - 1U - fontWidth + ((fontWidth - _bl.getWidth()) / 2U),
                     static_cast<int8_t>((GRID_COLUMNS / 2U) + static_cast<int8_t>(_bl.getHeight() > 5U)));
            _br.draw((GRID_COLUMNS / 2U) + 1U + ((fontWidth - _br.getWidth()) / 2U),
                     static_cast<int8_t>((GRID_COLUMNS / 2U) + static_cast<int8_t>(_br.getHeight() > 5U)));
            if (seconds == 0U && minutes == 0U && hours == 0U)
            {
                blink = INT8_MAX;
                odd = true;
                JsonDocument doc; // NOLINT(misc-const-correctness)
                doc["event"].set("done");
                Device.transmit(doc.as<JsonObjectConst>(), name, false);
            }
        }
    }
    else if (blink != 0U && odd == static_cast<bool>(static_cast<uint64_t>(seconds) & 1U))
    {
        --blink;
        odd = !odd;
        Display.invertFrame();
    }
}

void CountdownMode::save()
{
    blink = 0U;
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_i64(
            handle, "epoch", std::chrono::duration_cast<std::chrono::seconds>(epoch.time_since_epoch()).count());
        nvs_commit(handle);
        nvs_close(handle);
    }
    transmit();
}

void CountdownMode::setFont(std::string_view _fontName)
{
    if (const std::unique_ptr<const FontModule> _font{Fonts.get(_fontName)})
    {
        fontName = _font->name;
        nvs_handle_t handle{};
        if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
        {
            nvs_set_str(handle, "font", fontName.c_str());
            nvs_commit(handle);
            nvs_close(handle);
        }
        transmit();
    }
}

void CountdownMode::transmit()
{
    std::array<char, 32U> buffer{};
    time_t timer{std::chrono::system_clock::to_time_t(epoch)}; // NOLINT(cppcoreguidelines-init-variables)
    tm local{*std::localtime(&timer)};
    const size_t length{strftime(buffer.data(), buffer.size(), "%FT%T", &local)};
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["font"].set(fontName);
    JsonArray _fonts{doc["fonts"].to<JsonArray>()};
    for (const std::string_view _font : fontNames)
    {
        _fonts.add(_font);
    }
    doc["timestamp"].set(std::string_view(buffer.data(), min(buffer.size(), length)));
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void CountdownMode::onReceive(JsonObjectConst payload,
                              std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Font
    if (payload["font"].is<std::string_view>())
    {
        setFont(payload["font"].as<std::string_view>());
    }
    // Time or timestamp
    if (payload["time"].is<uint32_t>())
    {
        epoch = std::chrono::system_clock::now() + std::chrono::seconds(payload["time"].as<uint32_t>());
        save();
    }
    else if (payload["timestamp"].is<const char *>())
    {
        tm local{};
        strptime(payload["timestamp"].as<const char *>(), "%FT%T", &local);
        local.tm_isdst = -1;
        epoch = std::chrono::system_clock::from_time_t(mktime(&local));
        save();
    }
}

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void CountdownMode::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
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
        const std::string id{std::string(name).append("_timestamp")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(
            R"({"timestamp":"{{(value|as_datetime|as_local).strftime('%Y-%m-%dT%H:%M:%S')}}"})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:timer-sand-full");
        component[HomeAssistantAbbreviations::name].set(name);
        component[HomeAssistantAbbreviations::platform].set("datetime");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::timezone].set(TIME_ZONE);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.timestamp|as_datetime}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // MODE_COUNTDOWN
