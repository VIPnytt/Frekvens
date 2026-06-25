#if EXTENSION_PHOTOCELL

#include "extensions/PhotocellExtension.h"

#include "config/constants.h"                  // NOLINT(misc-include-cleaner)
#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"

#include <nvs.h>

void PhotocellExtension::configure() { pinMode(PIN_LDR, ANALOG); }

void PhotocellExtension::begin()
{
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        size_t length{sizeof(gamma)};
        nvs_get_blob(handle, "gamma", &gamma, &length);
        uint8_t _active{0U};
        if (nvs_get_u8(handle, "active", &_active) == ESP_OK)
        {
            nvs_close(handle);
            static_cast<bool>(_active) ? setActive(true) : transmit();
        }
        else
        {
            nvs_close(handle);
        }
    }
}

void PhotocellExtension::handle()
{
    if (pending || millis() - lastMillis > UINT16_MAX)
    {
        pending = false;
        if (!active || !Display.getPower())
        {
            raw = analogRead(PIN_LDR);
        }
        transmit();
    }
    else if (active && Display.getPower() && millis() - _lastMillis > UINT8_MAX)
    {
        _lastMillis = millis();
        raw = analogRead(PIN_LDR);
        // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
        const uint8_t _brightness{static_cast<uint8_t>(std::clamp<int16_t>(
            lroundf(((0b1U << 8U) + 1U) *
                        powf(static_cast<float>(raw + 1U) / static_cast<float>((0b1U << 12U) + 1U), gamma) -
                    1U),
            1,
            UINT8_MAX))};
        if ((direction && _brightness < brightness) || (!direction && _brightness > brightness))
        {
            direction = !direction;
            debounce /= 2;
        }
        debounce = static_cast<int16_t>(debounce + _brightness - brightness);
        if (abs(debounce) > UINT8_MAX)
        {
            brightness = _brightness;
            debounce = 0;
            Display.setBrightness(brightness);
        }
    }
}

bool PhotocellExtension::getActive() const { return active; }

void PhotocellExtension::setActive(bool _active)
{
    if (_active)
    {
        debounce = 0;
        brightness = Display.getBrightness();
    }
    active = _active;
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "active", static_cast<uint8_t>(active)); // NOLINT(readability-implicit-bool-conversion)
        nvs_commit(handle);
        nvs_close(handle);
    }
    pending = true;
    ESP_LOGI("Status", "%s", active ? "active" : "inactive"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
}

void PhotocellExtension::setGamma(float _gamma)
{
    gamma = _gamma;
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_blob(handle, "gamma", &gamma, sizeof(gamma));
        nvs_commit(handle);
        nvs_close(handle);
    }
}

void PhotocellExtension::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["active"].set(active);
    doc["illuminance"].set(raw);
    Device.transmit(doc.as<JsonObjectConst>(), name);
    lastMillis = millis();
}

void PhotocellExtension::onReceive(JsonObjectConst payload,
                                   std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Active
    if (payload["active"].is<bool>())
    {
        setActive(payload["active"].as<bool>());
    }
}

void PhotocellExtension::onTransmit(JsonObjectConst payload, std::string_view source)
{
    // Display: Brightness
    if (active && source == Display.name && payload["brightness"].is<uint8_t>())
    {
        // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
        const uint8_t _brightness{payload["brightness"].as<uint8_t>()};
        if (_brightness != brightness)
        {
            setGamma(logf(static_cast<float>(_brightness + 1U) / static_cast<float>((0b1U << 8U) + 1U)) /
                     logf(static_cast<float>(raw + 1U) / static_cast<float>((0b1U << 12U) + 1U)));
        }
    }
}

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void PhotocellExtension::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_active")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"active":{{value}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::icon].set("mdi:brightness-auto");
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
        const std::string id{std::string(name).append("_illuminance")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("diagnostic");
        component[HomeAssistantAbbreviations::icon].set("mdi:brightness-5");
        component[HomeAssistantAbbreviations::name].set("Illuminance");
        component[HomeAssistantAbbreviations::platform].set("sensor");
        component[HomeAssistantAbbreviations::state_class].set("measurement");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.illuminance}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // EXTENSION_PHOTOCELL
