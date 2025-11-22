#include "config/constants.h"

#if MODE_COUNTDOWN

#include <iomanip>
#include <Preferences.h>
#include <sstream>

#include "extensions/HomeAssistantExtension.h"
#include "fonts/MediumFont.h"
#include "handlers/TextHandler.h"
#include "modes/CountdownMode.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void CountdownMode::setup()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_timestamp");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"timestamp\":\"{{value}}\"}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::entity_category] = "config";
        component[HomeAssistantAbbreviations::icon] = "mdi:timer-sand-full";
        component[HomeAssistantAbbreviations::name] = name;
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::pattern] = R"(^(\d{4})-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01])T([01]\d|2[0-3]):[0-5]\d:[0-5]\d$)";
        component[HomeAssistantAbbreviations::platform] = "text";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.timestamp}}";
    }
#endif // EXTENSION_HOMEASSISTANT

    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("epoch"))
    {
        const int64_t _epoch = Storage.getLong64("epoch");
        Storage.end();
        epoch = std::chrono::system_clock::time_point{std::chrono::seconds{_epoch}};
        transmit();
    }
    else
    {
        Storage.end();
    }
}

void CountdownMode::wake()
{
    done = false;
}

void CountdownMode::handle()
{
    const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    const auto hours = std::chrono::duration_cast<std::chrono::hours>(epoch - now);
    const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(epoch - now - hours);
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch - now - hours - minutes);
    const uint8_t
        _upper = hours.count() > 99 ? 99 : (hours.count() > 0 ? hours.count() % 100 : minutes.count()),
        _lower = hours.count() > 99 ? 99 : (hours.count() > 0 ? minutes.count() : seconds.count());
    if (_lower != lower || _upper != upper)
    {
        upper = _upper;
        lower = _lower;
        if (seconds.count() >= 0 && minutes.count() >= 0 && hours.count() >= 0)
        {
            Display.clearFrame();
            TextHandler _tl = TextHandler(std::to_string(upper / 10), FontMedium);
            _tl.draw(GRID_COLUMNS / 2 - 1 - _tl.getWidth(), 0);
            TextHandler _tr = TextHandler(std::to_string(upper % 10), FontMedium);
            _tr.draw(GRID_COLUMNS / 2 + 1, 0);
            TextHandler _bl = TextHandler(std::to_string(lower / 10), FontMedium);
            _bl.draw(GRID_COLUMNS / 2 - 1 - _bl.getWidth(), GRID_ROWS - _bl.getHeight());
            TextHandler _br = TextHandler(std::to_string(lower % 10), FontMedium);
            _br.draw(GRID_COLUMNS / 2 + 1, GRID_ROWS - _br.getHeight());
            if (seconds.count() == 0 && minutes.count() == 0 && hours.count() == 0)
            {
                done = true;
                JsonDocument doc;
                doc["event"] = "done";
                Device.transmit(doc, name, false);
            }
        }
        else if (done)
        {
            Display.invertFrame();
        }
    }
}

void CountdownMode::transmit()
{
    time_t timer = std::chrono::system_clock::to_time_t(epoch);
    tm local = *std::localtime(&timer);
    std::ostringstream iso8601;
    iso8601 << std::put_time(&local, "%Y-%m-%dT%H:%M:%S");
    iso8601.str();

    JsonDocument doc;
    doc["timestamp"] = iso8601.str();
    Device.transmit(doc, name);
}

void CountdownMode::receiverHook(const JsonDocument doc, const char *const source)
{
    if (doc["time"].is<uint32_t>())
    {
        epoch = std::chrono::system_clock::now() + std::chrono::seconds(doc["time"].as<uint32_t>());
        save();
    }
    else if (doc["timestamp"].is<const char *>())
    {
        tm local;
        strptime(doc["timestamp"].as<const char *>(), "%FT%T", &local);
        local.tm_isdst = -1;
        epoch = std::chrono::system_clock::from_time_t(mktime(&local));
        save();
    }
}

void CountdownMode::save()
{
    Preferences Storage;
    Storage.begin(name);
    Storage.putLong64("epoch", std::chrono::duration_cast<std::chrono::seconds>(epoch.time_since_epoch()).count());
    Storage.end();
    transmit();
}

#endif // MODE_COUNTDOWN
