#if EXTENSION_PLAYLIST

#include "extensions/PlaylistExtension.h"

#include "services/DeviceService.h"
#include "services/DisplayService.h"    // NOLINT(misc-include-cleaner)
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)
#include "services/ModesService.h"

#include <nvs.h>

void PlaylistExtension::configure()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        size_t len{0};
        if (nvs_get_blob(handle, "modes", nullptr, &len) == ESP_OK && len > 0)
        {
            std::vector<uint8_t> buffer(len);
            if (nvs_get_blob(handle, "modes", &buffer, &len) == ESP_OK)
            {
                nvs_close(handle);
                if (deserializeJson(doc, buffer.data(), len) == DeserializationError::Code::Ok)
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
                nvs_close(handle);
            }
        }
    }
}

void PlaylistExtension::begin()
{
    nvs_handle_t handle{};
    const esp_reset_reason_t reason = esp_reset_reason();
    if (std::ranges::any_of(Device.resetAbnormalities, [&](esp_reset_reason_t _reason) { return _reason == reason; }))
    {
        if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
        {
            if (nvs_erase_key(handle, "active") == ESP_OK)
            {
                nvs_commit(handle);
            }
            nvs_close(handle);
        }
        transmit();
    }
    else if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        uint8_t _active{0};
        nvs_get_u8(handle, "active", &_active);
        nvs_close(handle);
        static_cast<bool>(_active) ? setActive(true) : transmit();
    }
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
        Modes.setMode(playlist[step].mode);
        lastMillis = millis();
    }
}

bool PlaylistExtension::getActive() const { return active; }

void PlaylistExtension::setActive(bool _active)
{
    if (_active && playlist.empty())
    {
        return;
    }
    step = 0;
    active = _active;
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "active", static_cast<uint8_t>(active));
        nvs_commit(handle);
        nvs_close(handle);
    }
    transmit();
    ESP_LOGI("Status", "%s", active ? "active" : "inactive"); // NOLINT(cppcoreguidelines-avoid-do-while)
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
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_blob(handle, "modes", buffer.data(), length + 1);
        nvs_commit(handle);
        nvs_close(handle);
    }
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

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void PlaylistExtension::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_active")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
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
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.active}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // EXTENSION_PLAYLIST
