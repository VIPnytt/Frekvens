#if EXTENSION_PHOTOCELL

#include "extensions/PhotocellExtension.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

#include <nvs.h>

void PhotocellExtension::configure() { pinMode(PIN_LDR, ANALOG); }

void PhotocellExtension::begin()
{
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        size_t len = sizeof(gamma);
        nvs_get_blob(handle, "gamma", &gamma, &len);
        uint8_t _active{0};
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
        const uint8_t _brightness = static_cast<uint8_t>(
            powf(static_cast<float>(raw) / static_cast<float>((1U << 12U) - 1), gamma) * UINT8_MAX);
        if ((direction && _brightness < brightness) || (!direction && _brightness > brightness))
        {
            direction = !direction;
            counter /= 2;
        }
        counter += _brightness - brightness;
        if (abs(counter) > UINT8_MAX)
        {
            brightness = _brightness;
            counter = 0;
            Display.setBrightness(brightness);
        }
    }
}

bool PhotocellExtension::getActive() const { return active; }

void PhotocellExtension::setActive(bool _active)
{
    if (_active)
    {
        counter = 0;
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
    ESP_LOGI("Status", "%s", active ? "active" : "inactive"); // NOLINT(cppcoreguidelines-avoid-do-while)
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
            setGamma(logf(static_cast<float>(_brightness) / static_cast<float>(1U << 8U)) /
                     logf(static_cast<float>(raw + 1) / static_cast<float>((1U << 12U) + 1)));
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
    {
        const std::string id{std::string(name).append("_illuminance")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("diagnostic");
        component[HomeAssistantAbbreviations::icon].set("mdi:brightness-5");
        component[HomeAssistantAbbreviations::name].set("Illuminance");
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::platform].set("sensor");
        component[HomeAssistantAbbreviations::state_class].set("measurement");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.illuminance}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // EXTENSION_PHOTOCELL
