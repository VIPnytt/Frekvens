#if EXTENSION_PLAYLIST

#include "extensions/PlaylistExtension.h"

#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)
#include "services/ModesService.h"

#include <nvs.h>

void PlaylistExtension::begin()
{
    const esp_reset_reason_t reason = esp_reset_reason();
    if (std::ranges::none_of(Device.resetAbnormalities, [&](esp_reset_reason_t _reason) { return _reason == reason; }))
    {
        nvs_handle_t handle{};
        if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
        {
            uint8_t _active{0};
            (nvs_get_u8(handle, "active", &_active) == ESP_OK && static_cast<bool>(_active)) ? setActive(true)
                                                                                             : transmit();
            nvs_close(handle);
        }
    }
    else
    {
        transmit();
    }
}

void PlaylistExtension::handle()
{
    if (active && Display.getPower() && millis() - lastMillis > duration)
    {
        nvs_handle_t handle{};
        if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
        {
            uint16_t _duration{0U};
            size_t length{0U}; // NOLINT(cppcoreguidelines-init-variables)
            const std::string _modeKey{std::string("mode").append(std::to_string(step))};
            if (nvs_get_str(handle, _modeKey.c_str(), nullptr, &length) == ESP_OK && length > 1U &&
                nvs_get_u16(handle, std::string("duration").append(std::to_string(step)).c_str(), &_duration) ==
                    ESP_OK &&
                _duration != 0U)
            {
                std::array<char, ModesService::namesMaxLength + 1U> _modeName{};
                length = _modeName.size();
                if (nvs_get_str(handle, _modeKey.c_str(), _modeName.data(), &length) == ESP_OK &&
                    std::ranges::find(ModesService::names, std::string_view{_modeName.data(), length - 1U}) !=
                        ModesService::names.end())
                {
                    duration = _duration * 1'000U;
                    mode.assign(_modeName.data(), length - 1U);
                    Modes.setMode(mode);
                    lastMillis = millis();
                }
                ++step;
            }
            else if (step == 0U)
            {
                setActive(false);
            }
            else
            {
                step = 0U;
            }
            nvs_close(handle);
        }
    }
}

bool PlaylistExtension::getActive() const { return active; }

void PlaylistExtension::setActive(bool _active)
{
    if (_active != active)
    {
        active = _active;
        nvs_handle_t handle{};
        if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
        {
            nvs_set_u8(handle, "active", static_cast<uint8_t>(active)); // NOLINT(readability-implicit-bool-conversion)
            nvs_commit(handle);
            nvs_close(handle);
        }
        transmit();
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        ESP_LOGI("Status", "%s", active ? "active" : "inactive");
    }
}

void PlaylistExtension::setPlaylist(std::span<const std::pair<std::string, uint16_t>> playlist)
{
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        size_t _step{0U};
        for (std::pair<std::string, uint16_t> item : playlist)
        {
            nvs_set_str(handle, std::string("mode").append(std::to_string(_step)).c_str(), item.first.c_str());
            nvs_set_u16(handle, std::string("duration").append(std::to_string(_step)).c_str(), item.second);
            ++_step;
        }
        while (_step <= UINT8_MAX &&
               nvs_find_key(handle, std::string("mode").append(std::to_string(_step)).c_str(), nullptr) == ESP_OK &&
               nvs_erase_key(handle, std::string("mode").append(std::to_string(_step)).c_str()) == ESP_OK &&
               nvs_erase_key(handle, std::string("duration").append(std::to_string(_step)).c_str()) == ESP_OK)
        {
            ++_step;
        }
        nvs_commit(handle);
        nvs_close(handle);
        transmit();
    }
}

void PlaylistExtension::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["active"].set(active);
    JsonArray playlist{doc["playlist"].to<JsonArray>()};
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        std::array<char, ModesService::namesMaxLength + 1U> _modeName{};
        for (size_t _step{0U}; _step <= UINT8_MAX; ++_step)
        {
            uint16_t _duration{0U};
            size_t length{_modeName.size()}; // NOLINT(cppcoreguidelines-init-variables)
            if (nvs_get_str(
                    handle, std::string("mode").append(std::to_string(_step)).c_str(), _modeName.data(), &length) ==
                    ESP_OK &&
                nvs_get_u16(handle, std::string("duration").append(std::to_string(_step)).c_str(), &_duration) ==
                    ESP_OK)
            {
                JsonObject item{playlist.add<JsonObject>()};
                item["duration"].set(_duration);
                item["mode"].set(std::string_view{_modeName.data(), length - 1U});
            }
            else
            {
                break;
            }
        }
        nvs_close(handle);
    }
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void PlaylistExtension::onTransmit(JsonObjectConst payload, std::string_view source)
{
    // Modes: Mode
    if (active && source == Modes.name && payload["mode"].is<std::string>() &&
        payload["mode"].as<std::string>() != mode)
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
        std::vector<std::pair<std::string, uint16_t>> playlist;
        for (const JsonVariantConst item : payload["playlist"].as<JsonArrayConst>())
        {
            if (item["mode"].is<std::string>() && item["duration"].is<uint16_t>())
            {
                playlist.push_back(
                    std::pair<std::string, uint16_t>(item["mode"].as<std::string>(), item["duration"].as<uint16_t>()));
            }
        }
        setPlaylist(playlist);
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
