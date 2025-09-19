#include "config/constants.h"

#if EXTENSION_MICROPHONE

#include <Preferences.h>

#include "extensions/HomeAssistantExtension.h"
#include "extensions/MicrophoneExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

MicrophoneExtension *Microphone = nullptr;

MicrophoneExtension::MicrophoneExtension() : ExtensionModule("Microphone")
{
    Microphone = this;
}

void MicrophoneExtension::setup()
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
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"active\":{{value}}}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::icon] = "mdi:microphone";
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
    {
        const std::string id = std::string(name).append("_sound");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::automation_type] = "trigger";
        component[Abbreviations::payload] = "sound";
        component[Abbreviations::platform] = "device_automation";
        component[Abbreviations::subtype] = name;
        component[Abbreviations::topic] = topic;
        component[Abbreviations::type] = "sound detected";
        component[Abbreviations::value_template] = "{{value_json.event}}";
    }
    {
        const std::string id = std::string(name).append("_threshold");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"threshold\":{{value}}}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::enabled_by_default] = false;
        component[Abbreviations::entity_category] = "config";
        component[Abbreviations::icon] = "mdi:microphone";
        component[Abbreviations::max] = levelMax;
        component[Abbreviations::min] = 1;
        component[Abbreviations::mode] = "slider";
        component[Abbreviations::name] = "Threshold";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::platform] = "number";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::value_template] = "{{value_json.threshold}}";
    }
#endif // EXTENSION_HOMEASSISTANT
}

void MicrophoneExtension::ready()
{
    Preferences Storage;
    Storage.begin(name, true);
    const bool _active = Storage.isKey("active") && Storage.getBool("active");
    if (Storage.isKey("threshold"))
    {
        threshold = Storage.getUShort("threshold");
    }
    Storage.end();
    _active ? set(true) : transmit();
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
        mic = analogReadRaw(PIN_MIC);
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
#ifdef F_VERBOSE
                Serial.printf("%s: sound, level %d\n", name, level);
#endif
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
#ifdef F_VERBOSE
            Serial.printf("%s: silence, level %d\n", name, level);
#endif
        }
    }
}

bool MicrophoneExtension::get()
{
    return active;
}

void MicrophoneExtension::set(bool enable)
{
    if ((enable && !active) || (!enable && active))
    {
        if (enable)
        {
            mic = analogReadRaw(PIN_MIC);
        }
        active = enable;
        Preferences Storage;
        Storage.begin(name);
        Storage.putBool("active", active);
        Storage.end();
        pending = true;

#ifdef F_INFO
        Serial.printf(active ? "%s: active\n" : "%s: inactive\n", name);
#endif
    }
}

void MicrophoneExtension::set(uint16_t floor)
{
    if (floor != threshold)
    {
        threshold = floor;
        Preferences Storage;
        Storage.begin(name);
        Storage.putUShort("threshold", threshold);
        Storage.end();
        pending = true;
    }
}

bool MicrophoneExtension::play()
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

void MicrophoneExtension::receiverHook(const JsonDocument doc)
{
    // Active
    if (doc["active"].is<bool>())
    {
        set(doc["active"].as<bool>());
    }
    // Threshold
    if (doc["threshold"].is<uint16_t>())
    {
        set(doc["threshold"].as<uint16_t>());
    }
}

#endif // EXTENSION_MICROPHONE
