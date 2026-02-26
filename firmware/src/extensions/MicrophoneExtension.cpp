#if EXTENSION_MICROPHONE

#include "extensions/MicrophoneExtension.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "extensions/HomeAssistantExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

#include <Preferences.h>

MicrophoneExtension *Microphone = nullptr; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

MicrophoneExtension::MicrophoneExtension() : ExtensionModule("Microphone") { Microphone = this; }

void MicrophoneExtension::configure()
{
    pinMode(PIN_MIC, ANALOG);

    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("max"))
    {
        levelMax = Storage.getUShort("max");
    }
    Storage.end();

#if EXTENSION_HOMEASSISTANT
    const std::string topic{std::string("frekvens/" HOSTNAME "/").append(name)};
    {
        const std::string id{std::string(name).append("_active")};
        JsonObject component{(*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};;
        component[HomeAssistantAbbreviations::command_template].set(R"({"active":{{value}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::icon].set("mdi:microphone");
        component[HomeAssistantAbbreviations::name].set(name);
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::payload_off].set("false");
        component[HomeAssistantAbbreviations::payload_on].set("true");
        component[HomeAssistantAbbreviations::platform].set("switch");
        component[HomeAssistantAbbreviations::state_off].set("False");
        component[HomeAssistantAbbreviations::state_on].set("True");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(HomeAssistant->uniquePrefix + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.active}}");
    }
    {
        const std::string id{std::string(name).append("_sound")};
        JsonObject component{(*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
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
        JsonObject component{(*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template] = R"({"threshold":{{value}}})";
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:microphone");
        component[HomeAssistantAbbreviations::max].set(levelMax);
        component[HomeAssistantAbbreviations::min].set(1);
        component[HomeAssistantAbbreviations::mode].set("slider");
        component[HomeAssistantAbbreviations::name].set("Threshold");
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::platform].set("number");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(HomeAssistant->uniquePrefix + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.threshold}}");
    }
#endif // EXTENSION_HOMEASSISTANT
}

void MicrophoneExtension::begin()
{
    Preferences Storage;
    Storage.begin(name, true);
    const bool _active = Storage.isKey("active") && Storage.getBool("active");
    if (Storage.isKey("threshold"))
    {
        threshold = Storage.getUShort("threshold");
    }
    Storage.end();
    _active ? setActive(true) : transmit();
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
        const uint16_t lastMic = mic;
        mic = analogRead(PIN_MIC);
        const uint16_t level = abs(mic - lastMic);
        if (level >= threshold)
        {
            lastMillis = millis();
            if (!detected)
            {
                detected = true;
                if (lastMillis - _lastMillis > UINT16_MAX)
                {
                    JsonDocument doc; // NOLINT(misc-const-correctness)
                    doc["event"].set("sound");
                    Device.transmit(doc.as<JsonObjectConst>(), name, false);
                    _lastMillis = lastMillis;
                }
                ESP_LOGV(name, "sound, level %d", level); // NOLINT(cppcoreguidelines-avoid-do-while)
            }
            else if (level > levelMax)
            {
                levelMax = level;
                Preferences Storage;
                Storage.begin(name);
                Storage.putUShort("max", levelMax);
                Storage.end();
                pending = true;
            }
        }
        else if (detected && millis() - lastMillis > INT8_MAX)
        {
            detected = false;
            ESP_LOGV(name, "silence, level %d", level); // NOLINT(cppcoreguidelines-avoid-do-while)
        }
    }
}

bool MicrophoneExtension::getActive() const { return active; }

void MicrophoneExtension::setActive(bool active)
{
    if ((active && !this->active) || (!active && this->active))
    {
        if (active)
        {
            mic = analogRead(PIN_MIC);
        }
        this->active = active;
        Preferences Storage;
        Storage.begin(name);
        Storage.putBool("active", this->active);
        Storage.end();
        pending = true;
        ESP_LOGI(name, "%s", this->active ? "active" : "inactive"); // NOLINT(cppcoreguidelines-avoid-do-while)
    }
}

void MicrophoneExtension::setThreshold(uint16_t _threshold)
{
    if (_threshold != threshold)
    {
        threshold = _threshold;
        Preferences Storage;
        Storage.begin(name);
        Storage.putUShort("threshold", threshold);
        Storage.end();
        pending = true;
    }
}

bool MicrophoneExtension::isTriggered() const { return detected || !active; }

void MicrophoneExtension::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["active"].set(active);
    doc["max"].set(levelMax);
    doc["threshold"].set(threshold);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void MicrophoneExtension::onReceive(JsonObjectConst payload,
                                    const char *source) // NOLINT(misc-unused-parameters)
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

#endif // EXTENSION_MICROPHONE
