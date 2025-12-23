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

void PlaylistExtension::configure()
{
    JsonDocument doc;
    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("modes"))
    {
        const size_t length = Storage.getBytesLength("modes");
        std::vector<uint8_t> buffer(length);
        Storage.getBytes("modes", buffer.data(), length);
        Storage.end();
        deserializeJson(doc, buffer.data(), length);
        JsonArrayConst modes = doc.as<JsonArrayConst>();
        playlist.reserve(modes.size());
        for (JsonVariantConst item : modes)
        {
            PlaylistExtension::Mode mode;
            mode.duration = item["duration"].as<uint16_t>();
            mode.mode = item["mode"].as<std::string>();
            playlist.push_back(mode);
        }
    }
    else
    {
        Storage.end();
    }
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_active");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"active\":{{value}}}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::icon] = "mdi:format-list-bulleted";
        component[HomeAssistantAbbreviations::json_attributes_template] = "{%set ns=namespace(d={})%}{%for i in value_json.playlist%}{%set ns.d=ns.d|combine({i.mode:i.duration})%}{%endfor%}{{ns.d}}";
        component[HomeAssistantAbbreviations::json_attributes_topic] = topic;
        component[HomeAssistantAbbreviations::name] = name;
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::payload_off] = "false";
        component[HomeAssistantAbbreviations::payload_on] = "true";
        component[HomeAssistantAbbreviations::platform] = "switch";
        component[HomeAssistantAbbreviations::state_off] = "False";
        component[HomeAssistantAbbreviations::state_on] = "True";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.active}}";
    }
#endif // EXTENSION_HOMEASSISTANT
}

void PlaylistExtension::begin()
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
    _active ? setActive(true) : transmit();
}

void PlaylistExtension::handle()
{
    if (active && Display.getPower() && millis() - lastMillis > 1'000 * playlist[step].duration)
    {
        ++step;
        if (step >= playlist.size())
        {
            step = 0;
        }
        Modes.setMode(playlist[step].mode.c_str());
        lastMillis = millis();
    }
}

bool PlaylistExtension::getActive() const
{
    return active;
}

void PlaylistExtension::setActive(bool active)
{
    if ((active && !this->active && !playlist.empty()) || (!active && this->active))
    {
        step = 0;
        this->active = active;
        Preferences Storage;
        Storage.begin(name);
        Storage.putBool("active", this->active);
        Storage.end();
        transmit();
        if (this->active)
        {
            ESP_LOGI(name, "active");
        }
        else
        {
            ESP_LOGI(name, "inactive");
        }
    }
}

void PlaylistExtension::setPlaylist(std::vector<PlaylistExtension::Mode> modes)
{
    setActive(false);
    playlist.clear();
    JsonDocument doc;
    JsonArray items = doc.to<JsonArray>();
    for (const Mode mode : modes)
    {
        JsonObject item;
        item["duration"] = mode.duration;
        item["mode"] = mode.mode;
        items.add(item);
        playlist.push_back(mode);
    }
    const size_t length = measureJson(doc);
    std::vector<uint8_t> buffer(length + 1);
    serializeJson(doc, reinterpret_cast<char *>(buffer.data()), length + 1);
    Preferences Storage;
    Storage.begin(name);
    Storage.putBytes("modes", buffer.data(), length + 1);
    Storage.end();
    transmit();
}

void PlaylistExtension::transmit()
{
    JsonDocument doc;
    doc["active"] = active;
    JsonArray _playlist = doc["playlist"].to<JsonArray>();
    for (const PlaylistExtension::Mode &mode : playlist)
    {
        JsonObject _item = _playlist.add<JsonObject>();
        _item["duration"] = mode.duration;
        _item["mode"] = mode.mode;
    }
    Device.transmit(doc, name);
}

void PlaylistExtension::onTransmit(const JsonDocument &doc, const char *const source)
{
    // Modes: Mode
    if (active && !strcmp(source, Modes.name) && doc["mode"].is<std::string>() && doc["mode"].as<std::string>() != playlist[step].mode)
    {
        setActive(false);
    }
}

void PlaylistExtension::onReceive(const JsonDocument doc, const char *const source)
{
    // Playlist
    if (doc["playlist"].is<JsonArrayConst>())
    {
        std::vector<Mode> _playlist;
        for (const JsonVariantConst item : doc["playlist"].as<JsonArrayConst>())
        {
            if (item["mode"].is<std::string>() && item["duration"].is<uint16_t>())
            {
                Mode mode;
                mode.duration = item["duration"].as<uint16_t>();
                mode.mode = item["mode"].as<std::string>();
                _playlist.push_back(mode);
            }
        }
        setPlaylist(_playlist);
    }
    // Active
    if (doc["active"].is<bool>())
    {
        setActive(doc["active"].as<bool>());
    }
}

#endif // EXTENSION_PLAYLIST
