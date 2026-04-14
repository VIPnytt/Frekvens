#if EXTENSION_PLAYLIST

#include "extensions/PlaylistExtension.h"

#include "extensions/HomeAssistantExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)
#include "services/ExtensionsService.h"
#include "services/ModesService.h"

#include <Preferences.h>

void PlaylistExtension::configure()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    Preferences Storage;
    Storage.begin(name.data(), true);
    if (Storage.isKey("modes"))
    {
        const size_t length = Storage.getBytesLength("modes");
        std::vector<uint8_t> buffer(length);
        Storage.getBytes("modes", buffer.data(), length);
        Storage.end();
        if (deserializeJson(doc, buffer.data(), length) == DeserializationError::Code::Ok)
        {
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
    }
    else
    {
        Storage.end();
    }
#if EXTENSION_HOMEASSISTANT
    const std::string topic{std::string("frekvens/" HOSTNAME "/").append(name)};
    const HomeAssistantExtension &_ha = Extensions.HomeAssistant();
    {
        const std::string id{std::string(name).append("_active")};
        JsonObject component{(*_ha.discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"active":{{value}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::icon].set("mdi:format-list-bulleted");
        component[HomeAssistantAbbreviations::json_attributes_template].set(
            "{%set ns=namespace(d={})%}{%for i in value_json.playlist%}{%set ns.d=ns.d|combine({i.mode:i.duration})%}{%endfor%}{{ns.d}}");
        component[HomeAssistantAbbreviations::json_attributes_topic].set(topic);
        component[HomeAssistantAbbreviations::name].set(name);
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::payload_off].set("false");
        component[HomeAssistantAbbreviations::payload_on].set("true");
        component[HomeAssistantAbbreviations::platform].set("switch");
        component[HomeAssistantAbbreviations::state_off].set("False");
        component[HomeAssistantAbbreviations::state_on].set("True");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(_ha.uniquePrefix + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.active}}");
    }
#endif // EXTENSION_HOMEASSISTANT
}

void PlaylistExtension::begin()
{
    bool _active = false;
    Preferences Storage;
    Storage.begin(name.data());
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

bool PlaylistExtension::getActive() const { return active; }

void PlaylistExtension::setActive(bool active)
{
    if ((active && !this->active && !playlist.empty()) || (!active && this->active))
    {
        step = 0;
        this->active = active;
        Preferences Storage;
        Storage.begin(name.data());
        Storage.putBool("active", this->active);
        Storage.end();
        transmit();
        ESP_LOGI(name, "%s", this->active ? "active" : "inactive"); // NOLINT(cppcoreguidelines-avoid-do-while)
    }
}

void PlaylistExtension::setPlaylist(std::span<PlaylistExtension::Mode> modes)
{
    setActive(false);
    playlist.clear();
    JsonDocument doc; // NOLINT(misc-const-correctness)
    JsonArray items{doc.to<JsonArray>()};
    for (const Mode mode : modes)
    {
        JsonObject item{items.add<JsonObject>()};
        item["duration"].set(mode.duration);
        item["mode"].set(mode.mode);
        playlist.push_back(mode);
    }
    const size_t length = measureJson(doc);
    std::vector<uint8_t> buffer(length + 1);
    serializeJson(doc, reinterpret_cast<char *>(buffer.data()), length + 1);
    Preferences Storage;
    Storage.begin(name.data());
    Storage.putBytes("modes", buffer.data(), length + 1);
    Storage.end();
    transmit();
}

void PlaylistExtension::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["active"].set(active);
    JsonArray _playlist{doc["playlist"].to<JsonArray>()};
    for (const PlaylistExtension::Mode &mode : playlist)
    {
        JsonObject _item{_playlist.add<JsonObject>()};
        _item["duration"].set(mode.duration);
        _item["mode"].set(mode.mode);
    }
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void PlaylistExtension::onTransmit(JsonObjectConst payload, std::string_view source)
{
    // Modes: Mode
    if (active && source == Modes.name && payload["mode"].is<std::string>() &&
        payload["mode"].as<std::string>() != playlist[step].mode)
    {
        setActive(false);
    }
}

void PlaylistExtension::onReceive(JsonObjectConst payload,
                                  std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Playlist
    if (payload["playlist"].is<JsonArrayConst>())
    {
        std::vector<Mode> _playlist;
        for (const JsonVariantConst item : payload["playlist"].as<JsonArrayConst>())
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
    if (payload["active"].is<bool>())
    {
        setActive(payload["active"].as<bool>());
    }
}

#endif // EXTENSION_PLAYLIST
