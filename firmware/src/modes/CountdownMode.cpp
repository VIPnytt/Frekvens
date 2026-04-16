#if MODE_COUNTDOWN

#include "modes/CountdownMode.h"

#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)
#include "services/FontsService.h"

#include <Preferences.h>
#include <array>
#include <iomanip>
#include <sstream>
#include <string_view>

void CountdownMode::configure()
{
    Preferences Storage;
    Storage.begin(name.data(), true);
    if (Storage.isKey("font"))
    {
        fontName = Storage.getString("font").c_str();
    }
    if (Storage.isKey("epoch"))
    {
        const int64_t _epoch = Storage.getLong64("epoch");
        Storage.end();
        epoch = std::chrono::system_clock::time_point{std::chrono::seconds{_epoch}};
    }
    else
    {
        Storage.end();
    }
    transmit();
}

void CountdownMode::begin()
{
    Preferences Storage;
    Storage.begin(name.data(), true);
    if (Storage.isKey("font"))
    {
        fontName = Storage.getString("font").c_str();
    }
    if (Storage.isKey("epoch"))
    {
        const int64_t _epoch = Storage.getLong64("epoch");
        Storage.end();
        epoch = std::chrono::system_clock::time_point{std::chrono::seconds{_epoch}};
    }
    else
    {
        Storage.end();
    }
    blink = 0;
    lower = 0;
    upper = 0;
}

void CountdownMode::handle()
{
    const std::chrono::nanoseconds _nanoseconds = epoch - std::chrono::system_clock::now();
    const std::chrono::hours _hours = std::chrono::duration_cast<std::chrono::hours>(_nanoseconds);
    const std::chrono::minutes _minutes = std::chrono::duration_cast<std::chrono::minutes>(_nanoseconds - _hours);
    const int64_t hours = _hours.count();
    const int64_t minutes = _minutes.count();
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
            const std::unique_ptr<const FontModule> font = Fonts.get(fontName);
            const TextHandler _tl(std::to_string(_upper / 10), *font);
            const TextHandler _tr(std::to_string(_upper % 10), *font);
            const TextHandler _bl(std::to_string(lower / 10), *font);
            const TextHandler _br(std::to_string(lower % 10), *font);
            const uint8_t fontWidth{max({_tl.getWidth(), _tr.getWidth(), _bl.getWidth(), _br.getWidth()})};
            Display.clearFrame();
            _tl.draw((GRID_COLUMNS / 2) - 1 - fontWidth + ((fontWidth - _tl.getWidth()) / 2),
                     (GRID_ROWS / 2) - 1 - _tl.getHeight());
            _tr.draw((GRID_COLUMNS / 2) + 1 + ((fontWidth - _tr.getWidth()) / 2),
                     (GRID_ROWS / 2) - 1 - _tr.getHeight());
            _bl.draw((GRID_COLUMNS / 2) - 1 - fontWidth + ((fontWidth - _bl.getWidth()) / 2),
                     (GRID_COLUMNS / 2) + static_cast<int8_t>(_bl.getHeight() > 5));
            _br.draw((GRID_COLUMNS / 2) + 1 + ((fontWidth - _br.getWidth()) / 2),
                     (GRID_COLUMNS / 2) + static_cast<int8_t>(_br.getHeight() > 5));
            if (seconds == 0 && minutes == 0 && hours == 0)
            {
                blink = INT8_MAX;
                odd = static_cast<bool>(seconds & 1);
                JsonDocument doc; // NOLINT(misc-const-correctness)
                doc["event"].set("done");
                Device.transmit(doc.as<JsonObjectConst>(), name, false);
            }
        }
    }
    else if (blink && odd == static_cast<bool>(seconds & 1))
    {
        --blink;
        odd = !odd;
        Display.invertFrame();
    }
}

void CountdownMode::save()
{
    blink = 0;
    Preferences preferences;
    preferences.begin(name.data());
    preferences.putLong64("epoch", std::chrono::duration_cast<std::chrono::seconds>(epoch.time_since_epoch()).count());
    preferences.end();
    transmit();
}

void CountdownMode::setFont(std::string_view _fontName)
{
    if (const std::unique_ptr<const FontModule> _font = Fonts.get(_fontName))
    {
        fontName = _font->name.data();
        Preferences Storage;
        Storage.begin(name.data());
        Storage.putString("font", fontName.c_str());
        Storage.end();
        transmit();
    }
}

void CountdownMode::transmit()
{
    std::array<char, 32> buffer{};
    time_t timer{std::chrono::system_clock::to_time_t(epoch)};
    tm local = *std::localtime(&timer);
    std::strftime(buffer.data(), buffer.size(), "%FT%T", &local);
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["font"].set(fontName);
    JsonArray _fonts{doc["fonts"].to<JsonArray>()};
    for (const std::string_view _font : fontNames)
    {
        _fonts.add(_font);
    }
    doc["timestamp"].set(std::string_view(buffer.data()));
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
        const std::string id{std::string(name).append("_timestamp")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"timestamp":"{{value}}"})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:timer-sand-full");
        component[HomeAssistantAbbreviations::name].set(name);
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::pattern].set(
            R"(^(\d{4})-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01])T([01]\d|2[0-3]):[0-5]\d:[0-5]\d$)");
        component[HomeAssistantAbbreviations::platform].set("text");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.timestamp}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // MODE_COUNTDOWN
