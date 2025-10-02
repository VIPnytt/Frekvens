#include "config/constants.h"

#if EXTENSION_PLAYLIST

#include <Preferences.h>

#include "extensions/HomeAssistantExtension.h"
#include "extensions/PlaylistExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ModesService.h"

PlaylistExtension *Playlist = nullptr;

PlaylistExtension::PlaylistExtension() : ExtensionModule("Playlist")
{
    Playlist = this;
}

void PlaylistExtension::setup()
{
    Preferences Storage;
    Storage.begin(name, true);
    uint8_t i = 0;
    while (Storage.isKey(std::string("mode" + std::to_string(i)).c_str()) && Storage.isKey(std::string("duration" + std::to_string(i)).c_str()))
    {
        PlaylistExtension::Item queued;
        queued.mode = Storage.getString(std::string("mode" + std::to_string(i)).c_str());
        queued.duration = Storage.getUShort(std::string("duration" + std::to_string(i)).c_str());
        playlist.push_back(queued);
        if (i >= UINT8_MAX)
        {
            break;
        }
        ++i;
    }
    Storage.end();

#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_active");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"active\":{{value}}}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::icon] = "mdi:format-list-bulleted";
        component[Abbreviations::json_attributes_template] = "{%set ns=namespace(d={})%}{%for i in value_json.playlist%}{%set ns.d=ns.d|combine({i.mode:i.duration})%}{%endfor%}{{ns.d}}";
        component[Abbreviations::json_attributes_topic] = topic;
        component[Abbreviations::name] = name;
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::payload_off] = "false";
        component[Abbreviations::payload_on] = "true";
        component[Abbreviations::platform] = "switch";
        component[Abbreviations::state_off] = "False";
        component[Abbreviations::state_on] = "True";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::value_template] = "{{value_json.active}}";
    }
#endif // EXTENSION_HOMEASSISTANT
}

void PlaylistExtension::ready()
{
    bool _active = false;
    Preferences Storage;
    Storage.begin(name);
    switch (esp_reset_reason())
    {
    case esp_reset_reason_t::ESP_RST_BROWNOUT:
    case esp_reset_reason_t::ESP_RST_INT_WDT:
    case esp_reset_reason_t::ESP_RST_PANIC:
    case esp_reset_reason_t::ESP_RST_TASK_WDT:
    case esp_reset_reason_t::ESP_RST_WDT:
        if (Storage.isKey("active"))
        {
            Storage.remove("active");
        }
        break;
    default:
        if (Storage.isKey("active") && Storage.getBool("active"))
        {
            _active = true;
        }
    }
    Storage.end();
    _active ? set(true) : transmit();
}

void PlaylistExtension::handle()
{
    if (active && Display.getPower() && millis() - lastMillis > 1000 * playlist[step].duration)
    {
        ++step;
        if (step >= playlist.size())
        {
            step = 0;
        }
#ifdef F_INFO
        Serial.printf("%s: next mode\n", name);
#endif
        Modes.set(playlist[step].mode.c_str());
        lastMillis = millis();
    }
}

bool PlaylistExtension::get()
{
    return active;
}

void PlaylistExtension::set(bool enable)
{
    if ((enable && !active && !playlist.empty()) || (!enable && active))
    {
        step = 0;
        active = enable;

        Preferences Storage;
        Storage.begin(name);
        Storage.putBool("active", active);
        Storage.end();

        transmit();

#ifdef F_INFO
        Serial.printf(active ? "%s: active\n" : "%s: inactive\n", name);
#endif
    }
}

void PlaylistExtension::load(std::vector<PlaylistExtension::Item> modes)
{
    set(false);
    playlist.clear();

    Preferences Storage;
    Storage.begin(name);
    Storage.clear();
    uint8_t i = 0;
    for (const Item mode : modes)
    {
        Storage.putUShort(std::string("duration" + std::to_string(i)).c_str(), mode.duration);
        Storage.putString(std::string("mode" + std::to_string(i)).c_str(), mode.mode);
        playlist.push_back(mode);
        ++i;
    }
    Storage.end();

    transmit();
}

void PlaylistExtension::transmit()
{
    JsonDocument doc;
    doc["active"] = active;
    JsonArray _playlist = doc["playlist"].to<JsonArray>();
    for (const PlaylistExtension::Item &mode : playlist)
    {
        JsonObject _item = _playlist.add<JsonObject>();
        _item["duration"] = mode.duration;
        _item["mode"] = mode.mode;
    }
    Device.transmit(doc, name);
}

void PlaylistExtension::transmitterHook(const JsonDocument &doc, const char *const source)
{
    // Modes: Mode
    if (active && !strcmp(source, Modes.name) && doc["mode"].is<String>() && !doc["mode"].as<String>().equals(playlist[step].mode))
    {
        set(false);
    }
}

void PlaylistExtension::receiverHook(const JsonDocument doc)
{
    // Playlist
    if (doc["playlist"].is<JsonArrayConst>())
    {
        std::vector<Item> _playlist;
        for (const JsonVariantConst queued : doc["playlist"].as<JsonArrayConst>())
        {
            if (queued["mode"].is<String>() && queued["duration"].is<uint16_t>())
            {
                Item item;
                item.duration = queued["duration"].as<uint16_t>();
                item.mode = queued["mode"].as<String>();
                _playlist.push_back(item);
            }
        }
        load(_playlist);
    }
    // Active
    if (doc["active"].is<bool>())
    {
        set(doc["active"].as<bool>());
    }
}

#endif // EXTENSION_PLAYLIST
