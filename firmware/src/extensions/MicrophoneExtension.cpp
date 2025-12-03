#if EXTENSION_MICROPHONE

#include <Preferences.h>

#include "config/constants.h"
#include "extensions/HomeAssistantExtension.h"
#include "extensions/MicrophoneExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

MicrophoneExtension *Microphone = nullptr;

MicrophoneExtension::MicrophoneExtension() : ExtensionModule("Microphone")
{
    Microphone = this;
}

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
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_active");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"active\":{{value}}}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::icon] = "mdi:microphone";
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
    {
        const std::string id = std::string(name).append("_sound");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::automation_type] = "trigger";
        component[HomeAssistantAbbreviations::payload] = "sound";
        component[HomeAssistantAbbreviations::platform] = "device_automation";
        component[HomeAssistantAbbreviations::subtype] = name;
        component[HomeAssistantAbbreviations::topic] = topic;
        component[HomeAssistantAbbreviations::type] = "sound detected";
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.event}}";
    }
    {
        const std::string id = std::string(name).append("_threshold");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"threshold\":{{value}}}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::enabled_by_default] = false;
        component[HomeAssistantAbbreviations::entity_category] = "config";
        component[HomeAssistantAbbreviations::icon] = "mdi:microphone";
        component[HomeAssistantAbbreviations::max] = levelMax;
        component[HomeAssistantAbbreviations::min] = 1;
        component[HomeAssistantAbbreviations::mode] = "slider";
        component[HomeAssistantAbbreviations::name] = "Threshold";
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::platform] = "number";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.threshold}}";
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
                    JsonDocument doc;
                    doc["event"] = "sound";
                    Device.transmit(doc, name, false);
                    _lastMillis = lastMillis;
                }
                ESP_LOGV(name, "sound, level %d", level);
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
            ESP_LOGV(name, "silence, level %d", level);
        }
    }
}

bool MicrophoneExtension::getActive()
{
    return active;
}

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

bool MicrophoneExtension::isTriggered() const
{
    return detected || !active;
}

void MicrophoneExtension::transmit()
{
    JsonDocument doc;
    doc["active"] = active;
    doc["max"] = levelMax;
    doc["threshold"] = threshold;
    Device.transmit(doc, name);
}

void MicrophoneExtension::onReceive(const JsonDocument doc, const char *const source)
{
    // Active
    if (doc["active"].is<bool>())
    {
        setActive(doc["active"].as<bool>());
    }
    // Threshold
    if (doc["threshold"].is<uint16_t>())
    {
        setThreshold(doc["threshold"].as<uint16_t>());
    }
}

#endif // EXTENSION_MICROPHONE
