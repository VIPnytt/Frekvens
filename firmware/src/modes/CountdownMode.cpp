#include <iomanip>
#include <Preferences.h>
#include <sstream>

#include "config/constants.h"
#include "extensions/HomeAssistantExtension.h"
#include "extensions/MqttExtension.h"
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
        const std::string id = std::string(name).append("_target");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"target\":\"{{value}}\"}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::entity_category] = "config";
        component[Abbreviations::icon] = "mdi:timer-sand-full";
        component[Abbreviations::name] = name;
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::pattern] = R"(^(\d{4})-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01])T([01]\d|2[0-3]):[0-5]\d:[0-5]\d$)";
        component[Abbreviations::platform] = "text";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::value_template] = "{{value_json.target}}";
    }
#endif // EXTENSION_HOMEASSISTANT

    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("target"))
    {
        const int64_t epoch = Storage.getLong64("target");
        Storage.end();
        target = std::chrono::system_clock::time_point{std::chrono::seconds{epoch}};
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
    const auto hours = std::chrono::duration_cast<std::chrono::hours>(target - now);
    const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(target - now - hours);
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(target - now - hours - minutes);
    if (hours.count() > 99)
    {
        _upper = 99;
        _lower = 99;
    }
    if (hours.count() > 0)
    {
        _upper = hours.count() % 100;
        _lower = minutes.count();
    }
    else
    {
        _upper = minutes.count();
        _lower = seconds.count();
    }
    if (_lower != lower || _upper != upper)
    {
        upper = _upper;
        lower = _lower;
        if (seconds.count() >= 0 && minutes.count() >= 0 && hours.count() >= 0)
        {
            Display.clear();
            TextHandler _tl = TextHandler(String(upper / 10), FontMedium);
            _tl.draw(COLUMNS / 2 - 1 - _tl.getWidth(), 0);
            TextHandler _tr = TextHandler(String(upper % 10), FontMedium);
            _tr.draw(COLUMNS / 2 + 1, 0);
            TextHandler _bl = TextHandler(String(lower / 10), FontMedium);
            _bl.draw(COLUMNS / 2 - 1 - _bl.getWidth(), ROWS - _bl.getHeight());
            TextHandler _br = TextHandler(String(lower % 10), FontMedium);
            _br.draw(COLUMNS / 2 + 1, ROWS - _br.getHeight());
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
            Display.invert();
        }
    }
}

void CountdownMode::transmit()
{
    time_t timer = std::chrono::system_clock::to_time_t(target);
    tm local = *std::localtime(&timer);
    std::ostringstream iso8601;
    iso8601 << std::put_time(&local, "%Y-%m-%dT%H:%M:%S");
    iso8601.str();

    JsonDocument doc;
    doc["target"] = iso8601.str();
    Device.transmit(doc, name);
}

void CountdownMode::receiverHook(const JsonDocument doc)
{
    if (doc["seconds"].is<uint32_t>())
    {
        target = std::chrono::system_clock::now() + std::chrono::seconds(doc["seconds"].as<uint32_t>());
        save();
    }
    else if (doc["target"].is<const char *>())
    {
        tm local;
        strptime(doc["target"].as<const char *>(), "%FT%T", &local);
        local.tm_isdst = -1;
        target = std::chrono::system_clock::from_time_t(mktime(&local));
        save();
    }
}

void CountdownMode::save()
{
    Preferences Storage;
    Storage.begin(name);
    Storage.putLong64("target", std::chrono::duration_cast<std::chrono::seconds>(target.time_since_epoch()).count());
    Storage.end();
    transmit();
}
