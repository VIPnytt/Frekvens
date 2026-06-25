#if EXTENSION_MICROPHONE

#include "extensions/MicrophoneExtension.h"

#include "config/constants.h"                  // NOLINT(misc-include-cleaner)
#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"

#include <nvs.h>

void MicrophoneExtension::configure()
{
    pinMode(PIN_MIC, ANALOG);
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        nvs_get_u16(handle, "ceiling", &soundCeiling);
        nvs_get_u8(handle, "floor", &soundFloor);
        nvs_close(handle);
    }
}

void MicrophoneExtension::begin()
{
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        uint8_t _active{0U};
        nvs_get_u8(handle, "active", &_active);
        nvs_get_u16(handle, "threshold", &threshold);
        nvs_close(handle);
        static_cast<bool>(_active) ? setActive(true) : transmit();
    }
}

void MicrophoneExtension::handle()
{
    if (pending)
    {
        pending = false;
        transmit();
    }
    else if (active && Display.getPower())
    {
        const uint16_t lastMic{mic};
        mic = analogRead(PIN_MIC);
        accumulated += abs(mic - lastMic);
        if (++index == UINT16_MAX / ((0b1U << 12U) - 1U))
        {
            const uint16_t activity{static_cast<uint16_t>(accumulated / (UINT16_MAX / ((0b1U << 12U) - 1U)))};
            if (activity > threshold)
            {
                if (!triggered)
                {
                    triggered = true;
                    if (millis() - lastMillis > UINT16_MAX)
                    {
                        JsonDocument doc; // NOLINT(misc-const-correctness)
                        doc["event"].set("sound");
                        Device.transmit(doc.as<JsonObjectConst>(), name, false);
                        lastMillis = millis();
                    }
                    ESP_LOGV("Sound", "level %d", activity); // NOLINT(cppcoreguidelines-pro-type-vararg)
                }
                else if (activity > soundCeiling)
                {
                    soundCeiling = activity;
                    nvs_handle_t handle{};
                    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
                    {
                        nvs_set_u16(handle, "ceiling", soundCeiling);
                        nvs_commit(handle);
                        nvs_close(handle);
                    }
                    pending = true;
                }
            }
            else if (triggered)
            {
                triggered = false;
                ESP_LOGV("Silence", "level %d", activity); // NOLINT(cppcoreguidelines-pro-type-vararg)
            }
            else if (activity < soundFloor)
            {
                soundFloor = activity;
                nvs_handle_t handle{};
                if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
                {
                    nvs_set_u8(handle, "floor", soundFloor);
                    nvs_commit(handle);
                    nvs_close(handle);
                }
                pending = true;
            }
            accumulated = 0U;
            index = 0U;
        }
    }
}

bool MicrophoneExtension::getActive() const { return active; }

void MicrophoneExtension::setActive(bool _active)
{
    if (active != _active)
    {
        if (_active)
        {
            accumulated = 0U;
            index = 0U;
            mic = analogRead(PIN_MIC);
        }
        active = _active;
        nvs_handle_t handle{};
        if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
        {
            nvs_set_u8(handle, "active", static_cast<uint8_t>(active)); // NOLINT(readability-implicit-bool-conversion)
            nvs_commit(handle);
            nvs_close(handle);
        }
        triggered = !active;
        pending = true;
        ESP_LOGI("Status", "%s", this->active ? "active" : "inactive"); // NOLINT(cppcoreguidelines-pro-type-vararg)
    }
}

void MicrophoneExtension::setThreshold(uint16_t _threshold)
{
    threshold = _threshold;
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u16(handle, "threshold", threshold);
        nvs_commit(handle);
        nvs_close(handle);
    }
    pending = true;
}

bool MicrophoneExtension::isTriggered() const { return triggered; }

void MicrophoneExtension::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["active"].set(active);
    doc["ceiling"].set(soundCeiling);
    doc["floor"].set(soundFloor);
    doc["threshold"].set(threshold);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void MicrophoneExtension::onReceive(JsonObjectConst payload,
                                    std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Active
    if (payload["active"].is<bool>())
    {
        setActive(payload["active"].as<bool>());
    }
    // Threshold
    if (payload["threshold"].is<uint16_t>())
    {
        setThreshold(payload["threshold"].as<uint16_t>());
    }
}

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void MicrophoneExtension::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_active")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"active":{{value}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::icon].set("mdi:microphone");
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
    {
        const std::string id{std::string(name).append("_sound")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::automation_type].set("trigger");
        component[HomeAssistantAbbreviations::payload].set("sound");
        component[HomeAssistantAbbreviations::platform].set("device_automation");
        component[HomeAssistantAbbreviations::subtype].set(name);
        component[HomeAssistantAbbreviations::topic].set(topic);
        component[HomeAssistantAbbreviations::type].set("sound detected");
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.event}}");
    }
    {
        const std::string id{std::string(name).append("_threshold")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"threshold":{{value}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:microphone");
        component[HomeAssistantAbbreviations::max].set(soundCeiling);
        component[HomeAssistantAbbreviations::min].set(soundFloor);
        component[HomeAssistantAbbreviations::mode].set("slider");
        component[HomeAssistantAbbreviations::name].set("Threshold");
        component[HomeAssistantAbbreviations::platform].set("number");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.threshold}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // EXTENSION_MICROPHONE
